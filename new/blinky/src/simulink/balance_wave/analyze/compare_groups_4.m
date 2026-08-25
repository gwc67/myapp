function compare_groups_4(root_path)
% COMPARE_GROUPS_4 对比多组实验数据，按target_speed分组合并
% 优化版：
%   1. 按target_speed创建子文件夹
%   2. 文件名包含group名称和target_speed后缀
%   3. 自动拆分同一文件中多个target_speed段
%   4. 处理时间重复问题
%
% 输入:
%   root_path - 根目录路径
% 输出:
%   在 root_path/group_[target_speed]/ 下生成文件，如：
%     spd_kp_0.27_ki_0.0043_tar40_pwma.csv
%     bal_kp_75_bal_kd_180_tar40_pitch.csv

    %% ========== 配置 ==========
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

    %% ========== 2. 收集所有CSV文件信息（自动拆分多target_speed段） ==========
    all_files = {};
    all_target_speeds = [];

    for g = 1:num_groups
        group_name = subfolders{g};
        group_path = fullfile(root_path, group_name);

        file_list = dir(fullfile(group_path, '*.csv'));
        group_segments = {};

        for f = 1:length(file_list)
            filepath = fullfile(group_path, file_list(f).name);

            try
                T = readtable(filepath);
                data = table2array(T);

                if size(data, 1) == 0
                    continue;
                end

                [~, name_no_ext, ~] = fileparts(file_list(f).name);
                seg_num = str2double(name_no_ext);
                if isnan(seg_num)
                    seg_num = 0;
                end

                % 自动检测并拆分同一文件中的多个target_speed段
                ts_col = data(:, col_target_speed);
                ts_diff = diff(ts_col);
                change_indices = find(abs(ts_diff) > 0.5);

                if isempty(change_indices)
                    change_indices = size(data, 1);
                else
                    change_indices = [change_indices; size(data, 1)];
                end

                start_idx = 1;
                for seg_idx = 1:length(change_indices)
                    end_idx = change_indices(seg_idx);
                    seg_data = data(start_idx:end_idx, :);

                    ts = seg_data(1, col_target_speed);
                    ts_rounded = target_round(ts);

                    seg_info.path = filepath;
                    seg_info.target_speed = ts;
                    seg_info.target_speed_rounded = ts_rounded;
                    seg_info.seg_num = seg_num * 1000 + seg_idx;
                    seg_info.group_name = group_name;
                    seg_info.data = seg_data;
                    seg_info.seg_idx = seg_idx;

                    group_segments{end+1} = seg_info;
                    all_target_speeds = [all_target_speeds; ts_rounded];

                    start_idx = end_idx + 1;
                end

            catch ME
                fprintf('  Warning: cannot read %s - %s\n', filepath, ME.message);
            end
        end

        all_files{g} = group_segments;
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

        % 创建该target_speed的输出文件夹
        output_folder_name = sprintf('group_%d', target_val);
        output_folder_path = fullfile(root_path, output_folder_name);
        if ~isfolder(output_folder_path)
            mkdir(output_folder_path);
            fprintf('  Created folder: %s\n', output_folder_path);
        end

        for c = 1:length(compare_cols)
            col_idx = compare_cols(c);
            col_name = compare_names{c};

            % 收集该target_speed下所有组的数据
            group_data_list = {};
            valid_groups = {};
            min_time_step = inf;
            max_duration = 0;

            for g = 1:num_groups
                group_segments = all_files{g};
                group_name = subfolders{g};

                % 找到该组中所有target_speed匹配的段
                matched_segments = {};
                for s = 1:length(group_segments)
                    if group_segments{s}.target_speed_rounded == target_val
                        matched_segments{end+1} = group_segments{s};
                    end
                end

                if isempty(matched_segments)
                    fprintf('  %s: no data for target_speed=%d\n', group_name, target_val);
                    continue;
                end

                % 合并多个匹配的段
                merged_data = matched_segments{1}.data;
                if length(matched_segments) > 1
                    fprintf('  %s: found %d segments, merging...\n', group_name, length(matched_segments));
                    for s = 2:length(matched_segments)
                        merged_data = [merged_data; matched_segments{s}.data];
                    end
                end

                % 确保时间单调递增并去除重复时间点
                time_col_data = merged_data(:, col_time);
                val_col_data = merged_data(:, col_idx);

                [time_col_data, sort_idx] = sort(time_col_data);
                val_col_data = val_col_data(sort_idx);

                [time_unique, unique_idx] = unique(time_col_data, 'stable');
                val_unique = val_col_data(unique_idx);

                num_removed = length(time_col_data) - length(time_unique);
                if num_removed > 0
                    fprintf('  %s: removed %d duplicate time points\n', group_name, num_removed);
                end

                time_col_data = time_unique;
                val_col_data = val_unique;
                num_samples = length(time_col_data);

                if num_samples < 2
                    fprintf('  %s: skipped, only %d sample(s) (need >=2)\n', group_name, num_samples);
                    continue;
                end

                if num_samples >= 2 && all(diff(time_col_data) == 0)
                    fprintf('  %s: skipped, all time values are identical\n', group_name);
                    continue;
                end

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

            if isinf(min_time_step) || min_time_step <= 0
                min_time_step = 0.005;
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

            % 对每个组的数据进行插值
            for g = 1:num_valid_groups
                gd = group_data_list{g};
                time_normalized = gd.time - gd.time(1);

                gd.val_aligned = interp1(time_normalized, gd.val, unified_time, 'linear', 'extrap');

                last_valid_time = time_normalized(end);
                last_valid_val = gd.val(end);
                beyond_idx = unified_time > last_valid_time;
                gd.val_aligned(beyond_idx) = last_valid_val;

                first_valid_time = time_normalized(1);
                first_valid_val = gd.val(1);
                before_idx = unified_time < first_valid_time;
                gd.val_aligned(before_idx) = first_valid_val;

                output_matrix(:, g+1) = gd.val_aligned;
            end

            % ===== 优化：导出为多个单独的文件，文件名包含group名称 =====
            for g = 1:num_valid_groups
                group_name = valid_groups{g};

                % 构建文件名: [group_name]_tar[target_speed]_[col_name].csv
                % 清理group_name中的非法字符
                safe_group_name = regexprep(group_name, '[^a-zA-Z0-9_\.]', '_');
                output_filename = sprintf('%s_tar%d_%s.csv', safe_group_name, target_val, col_name);
                output_filepath = fullfile(output_folder_path, output_filename);

                % 只导出该group的数据: [time, value]
                single_group_matrix = [unified_time, output_matrix(:, g+1)];
                T_out = array2table(single_group_matrix, 'VariableNames', {'time', col_name});
                writetable(T_out, output_filepath);

                fprintf('  -> Exported: %s\n', output_filepath);
            end

            % 同时导出一个合并对比文件（可选）
            merge_filename = sprintf('merge_tar%d_%s.csv', target_val, col_name);
            merge_filepath = fullfile(output_folder_path, merge_filename);
            T_merge = array2table(output_matrix, 'VariableNames', output_col_names);
            writetable(T_merge, merge_filepath);
            fprintf('  -> Merged: %s (%d groups)\n', merge_filepath, num_valid_groups);

            merge_files = dir(fullfile(output_folder_path, sprintf('merge_tar%d_*.csv', target_val)));
            %% 在每个 target_speed 分组处理完后，合并所有 merge 文件

            % 在 compare_groups_4 函数中，第 3 大循环的末尾添加：
            
            % 查找该 group 文件夹下所有 merge 文件
            merge_pattern = fullfile(output_folder_path, sprintf('merge_tar%d_*.csv', target_val));
            merge_files = dir(merge_pattern);
            
            %% 在每个 target_speed 分组处理完后，合并所有 merge 文件
            merge_pattern = fullfile(output_folder_path, sprintf('merge_tar%d_*.csv', target_val));
            merge_files = dir(merge_pattern);
            
        end
    fprintf('\nAll done! Files saved to: %s\n', root_path);
end