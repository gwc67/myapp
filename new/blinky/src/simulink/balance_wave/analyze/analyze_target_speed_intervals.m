function analyze_target_speed_intervals(folder_path)
% ANALYZE_TARGET_SPEED_INTERVALS 分析到达target_speed附近的时间间隔
%
% 输入:
%   folder_path - filter_raw导出的文件夹路径，例如 '.\data'
%
% 输出:
%   无返回值，在 folder_path\interval_analysis\ 目录下导出分析结果
%
% 分析逻辑:
%   1. 以 target_speed 突变的时间点作为分段起点
%   2. 在每个段内，找到 speed 进入 target_speed ± 2 范围的时刻
%   3. 计算从段起点到进入范围的时间间隔
%   4. 计算维持在 ± 2 范围内的持续时间
%   5. 计算整个段的平衡维持时间（running_success_flag=1的持续时间）
%
% 导出的列:
%   segment_id          - 段编号
%   start_time          - 段起始时间（target_speed突变点）
%   target_speed        - 该段的目标速度值
%   reach_time          - 首次到达 target_speed ± 2 的时间
%   reach_interval      - 到达目标附近的时间间隔（reach_time - start_time）
%   maintain_start      - 开始维持在 ± 2 范围内的时间
%   maintain_end        - 离开 ± 2 范围的时间
%   maintain_duration   - 维持在目标附近的时间
%   balance_start       - running_success_flag=1 开始时间
%   balance_end         - running_success_flag=1 结束时间
%   balance_duration    - 总平衡维持时间
%   max_speed_deviation - 该段内最大速度偏差
%
% 示例:
%   analyze_target_speed_intervals('.\log')

    %% ========== 配置 ==========
    % 输入文件的列名
    input_col_names = {'time', 'pwma', 'pwmb', 'pitch', 'motor_a_speed', ...
                       'motor_b_speed', 'target_speed', 'target_angle', 'running_success_flag'};

    % 列索引
    col_time = 1;
    col_motor_a = 5;
    col_motor_b = 6;
    col_target_speed = 7;
    col_flag = 9;

    % 阈值配置
    SPEED_TOLERANCE = 2;        % target_speed ± 2 的容差范围
    TARGET_SPEED_THRESHOLD = 0.5; % target_speed突变判定阈值（变化量超过此值视为突变）

    %% ========== 1. 检查输入目录 ==========
    if ~isfolder(folder_path)
        error('目录不存在: %s', folder_path);
    end

    file_list = dir(fullfile(folder_path, '*.csv'));
    num_files = length(file_list);

    if num_files == 0
        error('目录 %s 下没有找到CSV文件', folder_path);
    end

    fprintf('在 %s 下找到 %d 个CSV文件\n', folder_path, num_files);

    %% ========== 2. 创建输出目录 ==========
    output_dir = fullfile(folder_path, 'interval_analysis');
    if ~isfolder(output_dir)
        mkdir(output_dir);
        fprintf('创建分析输出目录: %s\n', output_dir);
    end

    %% ========== 3. 汇总所有段的分析结果 ==========
    all_results = [];
    global_seg_id = 0;

    for i = 1:num_files
        filename = file_list(i).name;
        filepath = fullfile(folder_path, filename);

        fprintf('\n处理: %s\n', filename);

        %% 3.1 读取数据
        try
            T = readtable(filepath);
            data = table2array(T);
        catch ME
            fprintf('  错误: 无法读取 %s - %s\n', filename, ME.message);
            continue;
        end

        num_rows = size(data, 1);
        if num_rows < 3
            fprintf('  跳过: 数据行数不足 (%d行)\n', num_rows);
            continue;
        end

        %% 3.2 提取各列
        time = data(:, col_time);
        motor_a = data(:, col_motor_a);
        motor_b = data(:, col_motor_b);
        target_speed = data(:, col_target_speed);
        flag = data(:, col_flag);

        % 计算平均轮速
        avg_speed = (motor_a + motor_b) / 2;

        %% 3.3 找到 target_speed 突变点（分段起点）
        % 计算 target_speed 的变化量
        target_speed_diff = diff(target_speed);

        % 找到突变点：变化量超过阈值的索引
       jump_indices = find(abs(target_speed_diff) > TARGET_SPEED_THRESHOLD);

        % 加上起始点（索引1）和结束点（最后一行）
        segment_boundaries = [1; jump_indices + 1; num_rows];
        segment_boundaries = unique(segment_boundaries);

        num_segments = length(segment_boundaries) - 1;
        fprintf('  找到 %d 个target_speed突变分段\n', num_segments);

        %% 3.4 逐段分析
        for seg = 1:num_segments
            start_idx = segment_boundaries(seg);
            end_idx = segment_boundaries(seg + 1) - 1;

            if end_idx <= start_idx
                continue;
            end

            seg_time = time(start_idx:end_idx);
            seg_speed = avg_speed(start_idx:end_idx);
            seg_target = target_speed(start_idx:end_idx);
            seg_flag = flag(start_idx:end_idx);

            seg_target_value = seg_target(1);  % 该段的目标速度值

            % 定义目标范围
            lower_bound = seg_target_value - SPEED_TOLERANCE;
            upper_bound = seg_target_value + SPEED_TOLERANCE;

            % 判断速度是否在目标范围内
            in_range = (seg_speed >= lower_bound) & (seg_speed <= upper_bound);

            % === 计算到达时间 ===
            reach_idx = find(in_range, 1, 'first');  % 首次进入范围的索引
            if isempty(reach_idx)
                reach_time = NaN;
                reach_interval = NaN;
                maintain_start = NaN;
                maintain_end = NaN;
                maintain_duration = 0;
            else
                reach_time = seg_time(reach_idx);
                reach_interval = reach_time - seg_time(1);

                % === 计算维持时间 ===
                % 找到连续在范围内的最长段
                diff_in_range = diff([0; in_range; 0]);
                range_start = find(diff_in_range == 1);
                range_end = find(diff_in_range == -1) - 1;

                if isempty(range_start)
                    maintain_start = NaN;
                    maintain_end = NaN;
                    maintain_duration = 0;
                else
                    % 计算每段持续时间，找最长的
                    durations = seg_time(range_end) - seg_time(range_start);
                    [max_duration, max_idx] = max(durations);

                    maintain_start = seg_time(range_start(max_idx));
                    maintain_end = seg_time(range_end(max_idx));
                    maintain_duration = max_duration;
                end
            end

            % === 计算平衡维持时间（running_success_flag=1）===
            valid_flag = (seg_flag == 1);
            if any(valid_flag)
                balance_start = seg_time(find(valid_flag, 1, 'first'));
                balance_end = seg_time(find(valid_flag, 1, 'last'));
                balance_duration = balance_end - balance_start;
            else
                balance_start = NaN;
                balance_end = NaN;
                balance_duration = 0;
            end

            % === 计算最大速度偏差 ===
            speed_error = abs(seg_speed - seg_target_value);
            max_speed_deviation = max(speed_error);

            % === 汇总结果 ===
            global_seg_id = global_seg_id + 1;

            result_row = [global_seg_id, ...
                          seg_time(1), ...
                          seg_target_value, ...
                          reach_time, ...
                          reach_interval, ...
                          maintain_start, ...
                          maintain_end, ...
                          maintain_duration, ...
                          balance_start, ...
                          balance_end, ...
                          balance_duration, ...
                          max_speed_deviation];

            all_results = [all_results; result_row];

            fprintf('  段 %d: target=%.2f, 到达间隔=%.3fs, 维持=%.3fs, 平衡=%.3fs\n', ...
                global_seg_id, seg_target_value, reach_interval, maintain_duration, balance_duration);
        end
    end

    %% ========== 4. 导出汇总结果 ==========
    if isempty(all_results)
        fprintf('\n没有有效的分析结果\n');
        return;
    end

    % 创建结果表
    result_col_names = {'segment_id', 'start_time', 'target_speed', ...
                        'reach_time', 'reach_interval', ...
                        'maintain_start', 'maintain_end', 'maintain_duration', ...
                        'balance_start', 'balance_end', 'balance_duration', ...
                        'max_speed_deviation'};

    T_result = array2table(all_results, 'VariableNames', result_col_names);

    % 导出汇总CSV
    summary_path = fullfile(output_dir, 'interval_summary.csv');
    writetable(T_result, summary_path);
    fprintf('\n汇总结果导出: %s\n', summary_path);

    % 导出每个段的详细数据（原始波形）
    % 这里可以扩展为导出每个段的子CSV

    %% ========== 5. 打印统计信息 ==========
    fprintf('\n========== 统计摘要 ==========\n');
    fprintf('总段数: %d\n', height(T_result));
    fprintf('平均到达间隔: %.3f s\n', mean(T_result.reach_interval, 'omitnan'));
    fprintf('最大到达间隔: %.3f s\n', max(T_result.reach_interval));
    fprintf('平均维持时间: %.3f s\n', mean(T_result.maintain_duration));
    fprintf('最大维持时间: %.3f s\n', max(T_result.maintain_duration));
    fprintf('平均平衡时间: %.3f s\n', mean(T_result.balance_duration));
    fprintf('最大平衡时间: %.3f s\n', max(T_result.balance_duration));
    fprintf('============================\n');
end