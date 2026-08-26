function compare_groups_6(root_path)
% COMPARE_GROUPS_6 对比多组实验数据，按target_speed分组合并
% 简化版：
%   1. 只导出 tar_[speed].csv 一个总文件
%   2. 跳过输出文件夹group_*避免重复扫描
%
% 输入:
%   root_path - 根目录路径
% 输出:
%   在 root_path/group_[target_speed]/ 下生成 tar_[speed].csv

    %% ========== 配置 ==========
    col_time = 1;
    col_pwma = 2;
    col_pwmb = 3;
    col_pitch = 4;
    col_motor_a = 5;
    col_motor_b = 6;
    col_target_speed = 7;
    col_target_angle = 8;
    % col_running_flag = 9;      % 新增：明确标注第9列
    col_angle_error = 10;      % 修复：从9改为10

    % 要对比的列
    compare_cols = [col_pwma, col_pwmb, col_pitch, col_motor_a, col_motor_b, col_target_angle,col_angle_error];
    compare_names = {'pwma', 'pwmb', 'pitch', 'motor_a_speed', 'motor_b_speed', 'target_angle','angle_error'};

    target_round = @(x) round(x);

    %% ========== 1. 扫描根目录下的所有子文件夹（排除输出文件夹） ==========
    if ~isfolder(root_path)
        error('Directory not found: %s', root_path);
    end

    dir_list = dir(root_path);
    subfolders = {};
    for i = 1:length(dir_list)
        if dir_list(i).isdir && ~strcmp(dir_list(i).name, '.') && ~strcmp(dir_list(i).name, '..')
            if startsWith(dir_list(i).name, 'group_')
                continue;
            end
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

    %% ========== 3. 按target_speed分组，处理每个速度组 ==========
    for t = 1:num_targets
        target_val = unique_targets(t);
        fprintf('\n--- Processing target_speed = %d ---\n', target_val);

        % 创建输出文件夹
        output_folder_name = sprintf('group_%d', target_val);
        output_folder_path = fullfile(root_path, output_folder_name);
        if ~isfolder(output_folder_path)
            mkdir(output_folder_path);
            fprintf('  Created folder: %s\n', output_folder_path);
        end

        % 收集所有指标的所有组数据
        all_group_data = cell(length(compare_cols), 1);
        all_valid_groups = cell(length(compare_cols), 1);
        all_min_time_step = inf(length(compare_cols), 1);
        all_max_duration = zeros(length(compare_cols), 1);

        for c = 1:length(compare_cols)
            col_idx = compare_cols(c);
            col_name = compare_names{c};

            group_data_list = {};
            valid_groups = {};
            min_time_step = inf;
            max_duration = 0;

            for g = 1:num_groups
                group_segments = all_files{g};
                group_name = subfolders{g};

                % 找到匹配的段
                matched_segments = {};
                for s = 1:length(group_segments)
                    if group_segments{s}.target_speed_rounded == target_val
                        matched_segments{end+1} = group_segments{s};
                    end
                end

                if isempty(matched_segments)
                    continue;
                end

                % 合并段
                merged_data = matched_segments{1}.data;
                for s = 2:length(matched_segments)
                    merged_data = [merged_data; matched_segments{s}.data];
                end

                % 处理时间和数值
                time_col_data = merged_data(:, col_time);
                val_col_data = merged_data(:, col_idx);

                [time_col_data, sort_idx] = sort(time_col_data);
                val_col_data = val_col_data(sort_idx);

                [time_unique, unique_idx] = unique(time_col_data, 'stable');
                val_unique = val_col_data(unique_idx);

                time_col_data = time_unique;
                val_col_data = val_unique;
                num_samples = length(time_col_data);

                if num_samples < 2 || all(diff(time_col_data) == 0)
                    continue;
                end

                dt = mean(diff(time_col_data));
                duration = time_col_data(end) - time_col_data(1);
                min_time_step = min(min_time_step, dt);
                max_duration = max(max_duration, duration);

                group_data_list{end+1} = struct('time', time_col_data, 'val', val_col_data, 'name', group_name);
                valid_groups{end+1} = group_name;
            end

            all_group_data{c} = group_data_list;
            all_valid_groups{c} = valid_groups;
            all_min_time_step(c) = min_time_step;
            all_max_duration(c) = max_duration;
        end

        % 找出全局最小时间步长和最大时长
        global_min_step = min(all_min_time_step(isfinite(all_min_time_step)));
        global_max_duration = max(all_max_duration);

        if isempty(global_min_step) || global_min_step <= 0
            global_min_step = 0.005;
        end

        unified_time = (0:global_min_step:global_max_duration)';
        num_rows = length(unified_time);

        % 构建总输出表
        all_col_names = {'time'};
        all_data = {unified_time};

        for c = 1:length(compare_cols)
            col_name = compare_names{c};
            group_data_list = all_group_data{c};

            for g = 1:length(group_data_list)
                gd = group_data_list{g};
                time_normalized = gd.time - gd.time(1);

                val_aligned = interp1(time_normalized, gd.val, unified_time, 'linear', 'extrap');

                % 边界处理
                last_valid_time = time_normalized(end);
                last_valid_val = gd.val(end);
                beyond_idx = unified_time > last_valid_time;
                val_aligned(beyond_idx) = last_valid_val;

                first_valid_time = time_normalized(1);
                first_valid_val = gd.val(1);
                before_idx = unified_time < first_valid_time;
                val_aligned(before_idx) = first_valid_val;

                % 列名: groupName_metricName
                safe_group_name = regexprep(gd.name, '[^a-zA-Z0-9_\.]', '_');
                new_col_name = sprintf('%s_%s', safe_group_name, col_name);
                all_col_names{end+1} = new_col_name;
                all_data{end+1} = val_aligned;
            end
        end

        % 导出总表
        tar_filename = sprintf('tar_%d.csv', target_val);
        tar_filepath = fullfile(output_folder_path, tar_filename);
        T_tar = table(all_data{:}, 'VariableNames', all_col_names);
        writetable(T_tar, tar_filepath);
        fprintf('  -> Exported: %s (%d columns)\n', tar_filepath, length(all_col_names));
    end

    fprintf('\nAll done! Files saved to: %s\n', root_path);
end