function filter_raw_batch_v5(folder_path)
% FILTER_RAW_BATCH_V5 批量处理文件夹下的所有CSV文件
% 自动适应列数，只需在 standard_col_names 中添加列名即可扩展
%
% 输入:
%   folder_path - 包含CSV文件的文件夹路径
%
% 输出:
%   在同级目录创建 [文件夹名]_filter/ 文件夹

    %% ========== 1. 预设标准列名（修改这里即可添加新列）==========
    standard_col_names = {'time', 'pitch', 'target_angle', 'pwma', 'motor_a_speed', ...
                          'target_speed', ...
                          'running_success_flag', 'e', 'ec','ec_raw'};
    
    % 自动计算列数
    num_standard_cols = length(standard_col_names);
    
    % 自动获取关键列的索引（修改列名顺序也不怕）
    flag_col = find(strcmp(standard_col_names, 'running_success_flag'));
    time_col = find(strcmp(standard_col_names, 'time'));
    target_speed_col = find(strcmp(standard_col_names, 'target_speed'));
    
    % 可选：获取其他可能用到的列索引
    % pitch_col = find(strcmp(standard_col_names, 'pitch'));
    % target_angle_col = find(strcmp(standard_col_names, 'target_angle'));
    % e_col = find(strcmp(standard_col_names, 'e'));
    % ec_col = find(strcmp(standard_col_names, 'ec'));

    TIME_PRECISION = 6;
    SPEED_CHANGE_THRESHOLD = 0.5;
    MIN_SAMPLES = 50;

    %% ========== 2. 检查输入文件夹 ==========
    if ~isfolder(folder_path)
        error('Folder not found: %s', folder_path);
    end

    %% ========== 3. 创建输出目录 ==========
    [parent_dir, folder_name, ~] = fileparts(folder_path);
    if isempty(parent_dir)
        parent_dir = '.';
    end

    export_dir = fullfile(parent_dir, [folder_name '_filter']);
    if ~isfolder(export_dir)
        mkdir(export_dir);
        fprintf('Created output dir: %s\n', export_dir);
    end

    %% ========== 4. 获取所有CSV文件 ==========
    file_list = dir(fullfile(folder_path, '*.csv'));
    num_files = length(file_list);

    fprintf('Found %d CSV files in %s\n', num_files, folder_path);
    fprintf('Standard columns (%d): ', num_standard_cols);
    fprintf('%s ', standard_col_names{:});
    fprintf('\n\n');

    if num_files == 0
        fprintf('No CSV files found\n');
        return;
    end

    %% ========== 5. 逐个处理CSV文件 ==========
    for f = 1:num_files
        csv_name = file_list(f).name;
        csv_path = fullfile(folder_path, csv_name);
        [~, csv_name_no_ext, ~] = fileparts(csv_name);

        fprintf('Processing: %s\n', csv_name);

        %% 5.1 读取CSV文件（自动判断是否有表头）
        fid = fopen(csv_path, 'r');
        first_line = fgetl(fid);
        fclose(fid);

        first_values = strsplit(first_line, ',');
        is_numeric = true;
        for i = 1:length(first_values)
            val = strtrim(first_values{i});
            if isnan(str2double(val)) && ~strcmp(val, 'NaN') && ~strcmp(val, 'Inf') && ~strcmp(val, '-Inf')
                is_numeric = false;
                break;
            end
        end

        if is_numeric
            data = readmatrix(csv_path);
        else
            data = readmatrix(csv_path, 'NumHeaderLines', 1);
        end

        num_cols = size(data, 2);

        %% 5.2 列数校验与裁剪
        if num_cols < num_standard_cols
            fprintf('  Skipped: %s has %d columns (need %d: %s)\n', ...
                csv_name, num_cols, num_standard_cols, strjoin(standard_col_names, ', '));
            continue;
        elseif num_cols > num_standard_cols
            fprintf('  Warning: %s has %d columns, keeping first %d\n', ...
                csv_name, num_cols, num_standard_cols);
            data = data(:, 1:num_standard_cols);
        end

        %% 5.3 找到 running_success_flag == 1 的连续段
        flag = data(:, flag_col);
        valid = (flag == 1);

        diff_valid = diff([0; valid; 0]);
        seg_start = find(diff_valid == 1);
        seg_end = find(diff_valid == -1) - 1;

        num_segments = length(seg_start);
        fprintf('  Found %d valid segments\n', num_segments);

        if num_segments == 0
            continue;
        end

        %% 5.4 创建该CSV的输出子目录
        csv_export_dir = fullfile(export_dir, csv_name_no_ext);
        if ~isfolder(csv_export_dir)
            mkdir(csv_export_dir);
        end

        %% 5.5 逐段处理
        export_count = 0;
        for i = 1:num_segments
            start_idx = seg_start(i);
            end_idx = seg_end(i);
            segment = data(start_idx:end_idx, :);

            % 时间归一化（从0开始）
            t0 = segment(1, time_col);
            segment(:, time_col) = segment(:, time_col) - t0;
            segment(:, time_col) = round(segment(:, time_col), TIME_PRECISION);

            % 按 target_speed 变化拆分
            ts_col = segment(:, target_speed_col);
            ts_diff = diff(ts_col);
            change_indices = find(abs(ts_diff) > SPEED_CHANGE_THRESHOLD);

            if isempty(change_indices)
                sub_segments = {segment};
            else
                change_indices = [change_indices; size(segment, 1)];
                sub_segments = {};
                sub_start = 1;
                for ci = 1:length(change_indices)
                    sub_end = change_indices(ci);
                    sub_segments{end+1} = segment(sub_start:sub_end, :);
                    sub_start = sub_end + 1;
                end
            end

            % 导出每个子段
            for j = 1:length(sub_segments)
                sub_seg = sub_segments{j};

                if size(sub_seg, 1) < MIN_SAMPLES
                    fprintf('    -> Skipped: only %d samples\n', size(sub_seg, 1));
                    continue;
                end

                ts_value = round(sub_seg(1, target_speed_col));
                duration = sub_seg(end, time_col) - sub_seg(1, time_col);

                export_count = export_count + 1;
                filename = sprintf('%03d_tar%d.csv', export_count, ts_value);
                filepath_out = fullfile(csv_export_dir, filename);

                T = array2table(sub_seg, 'VariableNames', standard_col_names);
                writetable(T, filepath_out);

                fprintf('    -> %s: target_speed=%d, duration=%.3fs, samples=%d\n', ...
                    filename, ts_value, duration, size(sub_seg, 1));
            end
        end

        fprintf('    Total exported: %d sub-segments\n', export_count);
    end

    fprintf('\nDone! All results in: %s\n', export_dir);
end