function analyze_segments(folder_path)
% ANALYZE_SEGMENTS 对filter_raw导出的分段CSV进行波形分析
%
% 输入:
%   folder_path - filter_raw导出的文件夹路径，例如 '.\data'
%                 该文件夹下包含 001.csv, 002.csv, ...
%
% 输出:
%   无返回值，在 folder_path\analyzed\ 目录下导出分析后的CSV文件
%   每段导出为 001_analyzed.csv, 002_analyzed.csv, ...
%
% 导出的列:
%   time        - 时间序列
%   speed_acc_a - A轮加速度 (motor_a_speed差分/dt)
%   speed_acc_b - B轮加速度 (motor_b_speed差分/dt)
%   pwm_acc_a   - PWM_A变化率 (pwma差分/dt)
%   pwm_acc_b   - PWM_B变化率 (pwmb差分/dt)
%   pitch_gyro  - 俯仰角角速度 (pitch差分/dt)
%
% 示例:
%   analyze_segments('D:\balance_car\data')
%   analyze_segments('.\log_20250825')

    %% ========== 配置 ==========
    % 输入文件的列名（filter_raw导出的标准列名）
    input_col_names = {'time', 'pwma', 'pwmb', 'pitch', 'motor_a_speed', ...
                       'motor_b_speed', 'target_speed', 'target_angle', 'running_success_flag'};

    % 输出文件的列名
    output_col_names = {'time', 'speed_acc_a', 'speed_acc_b', 'pwm_acc_a', 'pwm_acc_b', 'pitch_gyro'};

    % 列索引映射
    col_time = 1;
    col_pwma = 2;
    col_pwmb = 3;
    col_pitch = 4;
    col_motor_a = 5;
    col_motor_b = 6;

    %% ========== 1. 检查输入目录 ==========
    if ~isfolder(folder_path)
        error('目录不存在: %s', folder_path);
    end

    % 获取所有CSV文件
    file_list = dir(fullfile(folder_path, '*.csv'));
    num_files = length(file_list);

    if num_files == 0
        error('目录 %s 下没有找到CSV文件', folder_path);
    end

    fprintf('在 %s 下找到 %d 个CSV文件\n', folder_path, num_files);

    %% ========== 2. 创建输出目录 ==========
    output_dir = fullfile(folder_path, 'analyzed');
    if ~isfolder(output_dir)
        mkdir(output_dir);
        fprintf('创建分析输出目录: %s\n', output_dir);
    end

    %% ========== 3. 逐文件处理 ==========
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
        if num_rows < 2
            fprintf('  跳过: 数据行数不足 (%d行)\n', num_rows);
            continue;
        end

        %% 3.2 提取各列
        time = data(:, col_time);
        pwma = data(:, col_pwma);
        pwmb = data(:, col_pwmb);
        pitch = data(:, col_pitch);
        motor_a_speed = data(:, col_motor_a);
        motor_b_speed = data(:, col_motor_b);

        %% 3.3 计算时间间隔 dt
        dt = diff(time);  % dt(i) = time(i+1) - time(i)

        % 检查dt是否有异常值（如dt=0或负值）
        dt_min = min(dt);
        dt_max = max(dt);
        dt_mean = mean(dt);

        if dt_min <= 0
            fprintf('  警告: 时间序列存在非正间隔 (min_dt=%.6f)，将使用平均间隔%.6f\n', dt_min, dt_mean);
            dt(dt <= 0) = dt_mean;  % 用平均间隔替换异常值
        end

        %% 3.4 计算各项加速度/变化率
        % 使用中心差分或前向差分
        % 前向差分: (x(i+1) - x(i)) / dt(i)
        % 结果行数比原始少1行，对应时间点取两个点的中点或后一个点

        % 方法: 前向差分，结果对应 time(i+1) 时刻
        speed_acc_a = diff(motor_a_speed) ./ dt;
        speed_acc_b = diff(motor_b_speed) ./ dt;
        pwm_acc_a = diff(pwma) ./ dt;
        pwm_acc_b = diff(pwmb) ./ dt;
        pitch_gyro = diff(pitch) ./ dt;

        % 时间点: 取后一个点的时间（或中点）
        % 这里取后一个点的时间，即 time(2:end)
        time_out = time(2:end);

        %% 3.5 组装输出数据
        analyzed_data = [time_out, speed_acc_a, speed_acc_b, pwm_acc_a, pwm_acc_b, pitch_gyro];

        %% 3.6 导出CSV
        [~, name_no_ext, ~] = fileparts(filename);
        output_filename = sprintf('%s_analyzed.csv', name_no_ext);
        output_filepath = fullfile(output_dir, output_filename);

        T_out = array2table(analyzed_data, 'VariableNames', output_col_names);
        writetable(T_out, output_filepath);

        fprintf('  原始采样点数: %d -> 分析后: %d (差分损失1点)\n', num_rows, size(analyzed_data, 1));
        fprintf('  时间范围: %.3f ~ %.3f 秒\n', time_out(1), time_out(end));
        fprintf('  -> 导出: %s\n', output_filepath);
    end

    fprintf('\n全部完成！分析结果保存在: %s\n', output_dir);
end