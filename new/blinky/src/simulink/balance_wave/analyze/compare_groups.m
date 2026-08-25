function compare_groups(root_path)
% COMPARE_GROUPS 对比多组实验数据，按target_speed分组合并
%
% 输入:
%   root_path - 根目录路径，例如 'D:\balance_wave_raw'
%             目录结构:
%               balance_wave_raw/
%                 test/001.csv, 002.csv, ...
%                 test2/001.csv, 002.csv, ...
%                 test3/001.csv, 002.csv, ...
%
% 输出:
%   在root_path下生成 pwma_40.csv, pwma_60.csv, pitch_40.csv, ...
%   每个文件包含所有test组对应target_speed的数据
%
% 示例:
%   compare_groups('D:\balance_wave_raw')

    %% ========== 配置 ==========
    standard_col_names = {'time', 'pwma', 'pwmb', 'pitch', 'motor_a_speed', ...
                          'motor_b_speed', 'target_speed', 'target_angle', 'running_success_flag'};

    col_time = 1;
    col_pwma = 2;
    col_pwmb = 3;
    col_pitch = 4;
    col_motor_a = 5;
    col_motor_b = 6;
    col_target_speed = 7;
    col_target_angle = 8;
    col_flag = 9;

    % 要对比的列（除time和target_speed外）
    compare_cols = [col_pwma, col_pwmb, col_pitch, col_motor_a, col_motor_b, col_target_angle];
    compare_names = {'pwma', 'pwmb', 'pitch', 'motor_a_speed', 'motor_b_speed', 'target_angle'};

    % 取target_speed的整数部分作为分组标识
    target_round = @(x) round(x);

    %% ========== 1. 扫描根目录下的所有子文件夹 ==========
    if ~isfolder(root_path)
        error('Directory not found: %s', root_path);
    end

    dir_list = dir(root_path);
    subfolders = {};
    for i = 1:length(dir_list)
        if dir_list(i).isdir && ~strcmp(dir_list(i).name, '.') && ~strcmp(dir_list(i).name, '..')
            subfolders{end+1} = dir_list(i).name;
        end
    end

    num_groups = length(subfolders);
    fprintf('Found %d subfolders: ', num_groups);
    fprintf('%s ', subfolders{:});
    fprintf('\n');

    if num_groups == 0
        fprintf('No subfolders found in %s\n', root_path);
        return;
    end

    %% ========== 2. 收集所有CSV文件信息 ==========
    % 结构: all_files{group_idx}{file_idx} = struct with fields: path, target_speed, seg_num
    all_files = {};
    all_target_speeds = [];

    for g = 1:num_groups
        group_name = subfolders{g};
        group_path = fullfile(root_path, group_name);

        file_list = dir(fullfile(group_path, '*.csv'));
        group_files = {};

        for f = 1:length(file_list)
            filepath = fullfile(group_path, file_list(f).name);

            % 读取数据获取target_speed
            try
                T = readtable(filepath);
                data = table2array(T);

                if size(data, 1) == 0
                    continue;
                end

                % 取该段的target_speed（假设段内target_speed基本不变）
                ts = data(1, col_target_speed);
                ts_rounded = target_round(ts);

                % 解析段号 (001.csv -> 1)
                [~, name_no_ext, ~] = fileparts(file_list(f).name);
                seg_num = str2double(name_no_ext);

                file_info.path = filepath;
                file_info.target_speed = ts;
                file_info.target_speed_rounded = ts_rounded;
                file_info.seg_num = seg_num;
                file_info.group_name = group_name;
                file_info.data = data;

                group_files{end+1} = file_info;
                all_target_speeds = [all_target_speeds; ts_rounded];

            catch ME
                fprintf('  Warning: cannot read %s - %s\n', filepath, ME.message);
            end
        end

        all_files{g} = group_files;
    end

    % 获取所有唯一的target_speed分组
    unique_targets = unique(all_target_speeds);
    num_targets = length(unique_targets);

    fprintf('Found %d unique target_speed groups: ', num_targets);
    fprintf('%d ', unique_targets);
    fprintf('\n');

    if num_targets == 0
        fprintf('No valid data found\n');
        return;
    end

    %% ========== 3. 按target_speed分组，合并各组数据 ==========
    for t = 1:num_targets
        target_val = unique_targets(t);
        fprintf('\n--- Processing target_speed = %d ---\n', target_val);

        % 对每个要对比的列，收集所有组的数据
        for c = 1:length(compare_cols)
            col_idx = compare_cols(c);
            col_name = compare_names{c};

            % 收集该target_speed下所有组的数据
            % 格式: merged_data = [time, group1_val, group2_val, group3_val, ...]
            merged_data = [];
            max_rows = 0;
            group_data_list = {};
            valid_groups = {};

            for g = 1:num_groups
                group_files = all_files{g};
                group_name = subfolders{g};

                % 找到该组中target_speed匹配的文件
                matched_file = [];
                for f = 1:length(group_files)
                    if group_files{f}.target_speed_rounded == target_val
                        matched_file = group_files{f};
                        break;
                    end
                end

                if isempty(matched_file)
                    fprintf('  %s: no data for target_speed=%d\n', group_name, target_val);
                    continue;
                end

                data = matched_file.data;
                time_col_data = data(:, col_time);
                val_col_data = data(:, col_idx);

                group_data_list{end+1} = struct('time', time_col_data, 'val', val_col_data, 'name', group_name);
                valid_groups{end+1} = group_name;
                max_rows = max(max_rows, length(time_col_data));

                fprintf('  %s: %s, samples=%d, duration=%.3fs\n', ...
                    group_name, matched_file.path, length(time_col_data), time_col_data(end)-time_col_data(1));
            end

            num_valid_groups = length(group_data_list);
            if num_valid_groups == 0
                fprintf('  No valid groups for %s at target_speed=%d\n', col_name, target_val);
                continue;
            end

            % 对齐时间轴：取最长的时间轴，其他组用NaN填充
            % 找到参考时间轴（取第一组的时间）
            ref_time = group_data_list{1}.time;

            % 构建输出矩阵: [time, group1, group2, group3, ...]
            output_matrix = nan(max_rows, 1 + num_valid_groups);
            output_matrix(1:length(ref_time), 1) = ref_time;

            % 列名
            output_col_names = {'time'};
            for g = 1:num_valid_groups
                output_col_names{end+1} = valid_groups{g};
            end

            % 填充数据
            for g = 1:num_valid_groups
                gd = group_data_list{g};
                n = length(gd.time);

                % 如果该组的时间轴与参考不同，需要插值对齐
                if n ~= length(ref_time) || any(abs(gd.time - ref_time(1:n)) > 1e-6)
                    % 使用插值对齐到参考时间轴
                    gd.val_aligned = interp1(gd.time, gd.val, ref_time, 'linear', 'extrap');
                else
                    gd.val_aligned = gd.val;
                end

                output_matrix(:, g+1) = gd.val_aligned;
            end

            % 导出CSV
            output_filename = sprintf('%s_%d.csv', col_name, target_val);
            output_filepath = fullfile(root_path, output_filename);

            T_out = array2table(output_matrix, 'VariableNames', output_col_names);
            writetable(T_out, output_filepath);

            fprintf('  -> Exported: %s (%d groups, %d rows)\n', output_filepath, num_valid_groups, max_rows);
        end
    end

    fprintf('\nAll done! Files saved to: %s\n', root_path);
end