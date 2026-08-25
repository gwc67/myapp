function filter_raw_batch_2(folder_path)
% FILTER_RAW_BATCH 批量处理文件夹下的所有CSV文件
%
% 输入:
%   folder_path - 包含CSV文件的文件夹路径，例如 './a/b/c'
%
% 输出:
%   在同级目录创建 c_filter/ 文件夹
%   每个CSV文件的结果放在 c_filter/文件名/001.csv, 002.csv, ...
%
% 示例:
%   filter_raw_batch('./a/b/c')
%   -> 输出到 ./a/b/c_filter/test/001.csv, test2/001.csv, ...

    %% ========== 预设标准列名和列顺序 ==========
    standard_col_names = {'time', 'pwma', 'pwmb', 'pitch', 'motor_a_speed', ...
                          'motor_b_speed', 'target_speed', 'target_angle', 'running_success_flag'};

    flag_col = 9;
    time_col = 1;
    TIME_PRECISION = 6;

    %% ========== 1. 检查输入文件夹 ==========
    if ~isfolder(folder_path)
        error('Folder not found: %s', folder_path);
    end

    %% ========== 2. 创建输出目录（同级目录 + _filter后缀）==========
    [parent_dir, folder_name, ~] = fileparts(folder_path);

    % 处理 ./a/b/c 这种路径，确保parent_dir正确
    if isempty(parent_dir)
        parent_dir = '.';
    end

    export_dir = fullfile(parent_dir, [folder_name '_filter']);
    if ~isfolder(export_dir)
        mkdir(export_dir);
        fprintf('Created output dir: %s\n', export_dir);
    end

    %% ========== 3. 获取当前文件夹下的所有CSV文件（不包括子文件夹）==========
    file_list = dir(fullfile(folder_path, '*.csv'));
    num_files = length(file_list);

    fprintf('Found %d CSV files in %s\n', num_files, folder_path);

    if num_files == 0
        fprintf('No CSV files found\n');
        return;
    end

    %% ========== 4. 逐个处理CSV文件 ==========
    for f = 1:num_files
        csv_name = file_list(f).name;
        csv_path = fullfile(folder_path, csv_name);
        [~, csv_name_no_ext, ~] = fileparts(csv_name);

        fprintf('\nProcessing: %s\n', csv_name);

        %% 4.1 读取CSV文件
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
        num_standard = length(standard_col_names);

        if num_cols < num_standard
            fprintf('  Skipped: %s has only %d columns (need %d)\n', csv_name, num_cols, num_standard);
            continue;
        elseif num_cols > num_standard
            data = data(:, 1:num_standard);
        end

        %% 4.2 找到 running_success_flag == 1 的连续段
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

        %% 4.3 创建该CSV的输出子目录
        csv_export_dir = fullfile(export_dir, csv_name_no_ext);
        if ~isfolder(csv_export_dir)
            mkdir(csv_export_dir);
        end

        %% 4.4 逐段导出（时间从0开始，四舍五入）
        for i = 1:num_segments
            start_idx = seg_start(i);
            end_idx = seg_end(i);

            segment = data(start_idx:end_idx, :);

            % 时间归一化
            t0 = segment(1, time_col);
            segment(:, time_col) = segment(:, time_col) - t0;
            segment(:, time_col) = round(segment(:, time_col), TIME_PRECISION);

            seg_time = segment(:, time_col);
            duration = seg_time(end) - seg_time(1);

            fprintf('    Seg %d: idx[%d:%d], duration %.3fs, samples %d\n', ...
                i, start_idx, end_idx, duration, size(segment, 1));

            filename = sprintf('%03d.csv', i);
            filepath_out = fullfile(csv_export_dir, filename);

            T = array2table(segment, 'VariableNames', standard_col_names);
            writetable(T, filepath_out);
        end

        fprintf('    -> Exported %d segments to %s\n', num_segments, csv_export_dir);
    end

    fprintf('\nDone! All results in: %s\n', export_dir);
end