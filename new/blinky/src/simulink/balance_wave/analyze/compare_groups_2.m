function compare_groups_2(root_path)
% COMPARE_GROUPS 对比多组实验数据，按target_speed分组合并
%
% 输入:
%   root_path - 根目录路径
% 输出:
%   在root_path下生成 pwma_40.csv, pitch_40.csv 等对比文件
%   超出原始数据范围的部分用最后一次有效值填充（不是NaN）

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

    % 要对比的列
    compare_cols = [col_pwma, col_pwmb, col_pitch, col_motor_a, col_motor_b, col_target_angle];
    compare_names = {'pwma', 'pwmb', 'pitch', 'motor_a_speed', 'motor_b_speed', 'target_angle'};

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
    all_files = {};
    all_target_speeds = [];

    for g = 1:num_groups
        group_name = subfolders{g};
        group_path = fullfile(root_path, group_name);

        file_list = dir(fullfile(group_path, '*.csv'));
        group_files = {};

        for f = 1:length(file_list)
            filepath = fullfile(group_path, file_list(f).name);

            try
                T = readtable(filepath);
                data = table2array(T);

                if size(data, 1) == 0
                    continue;
                end

                ts = data(1, col_target_speed);
                ts_rounded = target_round(ts);

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

        for c = 1:length(compare_cols)
            col_idx = compare_cols(c);
            col_name = compare_names{c};

            % 收集该target_speed下所有组的数据
            group_data_list = {};
            valid_groups = {};
            min_time_step = inf;
            max_duration = 0;

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
                num_samples = length(time_col_data);

                % 跳过数据点太少的文件（至少需要2个点才能插值）
                if num_samples < 2
                    fprintf('  %s: skipped, only %d sample(s) (need >=2)\n', group_name, num_samples);
                    continue;
                end

                % 检查时间是否有变化（如果所有时间相同，也无法插值）
                if num_samples >= 2 && all(diff(time_col_data) == 0)
                    fprintf('  %s: skipped, all time values are identical\n', group_name);
                    continue;
                end

                % 计算时间步长和持续时间
                dt = mean(diff(time_col_data));
                duration = time_col_data(end) - time_col_data(1);
                min_time_step = min(min_time_step, dt);
                max_duration = max(max_duration, duration);

                group_data_list{end+1} = struct('time', time_col_data, 'val', val_col_data, 'name', group_name);
                valid_groups{end+1} = group_name;

                fprintf('  %s: samples=%d, duration=%.3fs\n', ...
                    group_name, num_samples, duration);
            end

            num_valid_groups = length(group_data_list);
            if num_valid_groups == 0
                fprintf('  No valid groups for %s at target_speed=%d\n', col_name, target_val);
                continue;
            end

            % 生成统一的时间轴（从0开始，步长为最小时间步长，覆盖最大持续时间）
            if isinf(min_time_step) || min_time_step <= 0
                min_time_step = 0.005;  % 默认5ms
            end

            unified_time = (0:min_time_step:max_duration)';
            num_rows = length(unified_time);

            % 构建输出矩阵: [time, group1, group2, group3, ...]
            output_matrix = zeros(num_rows, 1 + num_valid_groups);
            output_matrix(:, 1) = unified_time;

            output_col_names = {'time'};
            for g = 1:num_valid_groups
                output_col_names{end+1} = valid_groups{g};
            end

            % 对每个组的数据进行插值，对齐到统一时间轴
            for g = 1:num_valid_groups
                gd = group_data_list{g};

                % 使用线性插值对齐到统一时间轴
                % 超出原始数据范围的部分用最后一次有效值填充（不是NaN）
                gd.val_aligned = interp1(gd.time, gd.val, unified_time, 'linear', 'extrap');

                % 对于超出原始数据范围的部分，用最后一次有效值替换外推值
                last_valid_time = gd.time(end);
                last_valid_val = gd.val(end);
                beyond_idx = unified_time > last_valid_time;
                gd.val_aligned(beyond_idx) = last_valid_val;

                % 对于时间小于第一个数据点的情况，用第一个值填充
                first_valid_time = gd.time(1);
                first_valid_val = gd.val(1);
                before_idx = unified_time < first_valid_time;
                gd.val_aligned(before_idx) = first_valid_val;

                output_matrix(:, g+1) = gd.val_aligned;
            end

            % 导出CSV
            output_filename = sprintf('%s_%d.csv', col_name, target_val);
            output_filepath = fullfile(root_path, output_filename);

            T_out = array2table(output_matrix, 'VariableNames', output_col_names);
            writetable(T_out, output_filepath);

            fprintf('  -> Exported: %s (%d groups, %d rows, dt=%.4fs)\n', ...
                output_filepath, num_valid_groups, num_rows, min_time_step);
        end
    end

    fprintf('\nAll done! Files saved to: %s\n', root_path);
end