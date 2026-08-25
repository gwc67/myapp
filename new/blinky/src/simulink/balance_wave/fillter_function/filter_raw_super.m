function filter_raw_super(filepath)
% FILTER_RAW 读取CSV文件，根据running_success_flag=1拆分数据段，分别导出CSV
%
% 输入:
%   filepath - CSV文件路径
%
% 特性:
%   - 优先尝试匹配列名
%   - 若列名被刷掉/无法匹配，使用预设的默认列名和列顺序
%   - 导出的CSV始终使用预设的标准列名（不是原始列名）
%
% 示例:
%   filter_raw('data.csv')

    %% ========== 预设标准列名和列顺序 ==========
    % 这是导出的CSV将使用的标准列名（按顺序对应CSV的第1,2,3...列）
    standard_col_names = {'time', 'pwma', 'pwmb', 'pitch', 'motor_a_speed', ...
                          'motor_b_speed', 'target_speed', 'target_angle', 'running_success_flag'};

    % 关键列在标准列名中的位置（从1开始计数）
    flag_col = 9;   % running_success_flag 在第9列
    time_col = 1;   % time 在第1列

    %% ========== 1. 读取CSV文件 ==========
    if ~isfile(filepath)
        error('文件不存在: %s', filepath);
    end

    % 尝试读取第一行，判断是否有表头
    fid = fopen(filepath, 'r');
    first_line = fgetl(fid);
    fclose(fid);

    % 检查第一行是否为数字（无表头）还是文本（有表头）
    first_values = strsplit(first_line, ',');
    is_numeric = true;
    for i = 1:length(first_values)
        val = strtrim(first_values{i});
        if isnan(str2double(val)) && ~strcmp(val, 'NaN') && ~strcmp(val, 'Inf') && ~strcmp(val, '-Inf')
            is_numeric = false;
            break;
        end
    end

    %% ========== 2. 读取数据 ==========
    if is_numeric
        % 第一行是数字，说明没有表头
        fprintf('检测到无表头，使用标准列名\n');
        % 读取所有数据（包括第一行）
        data = readmatrix(filepath);
    else
        % 第一行是文本表头，跳过表头行读取数据
        fprintf('检测到有表头，跳过表头读取数据\n');
        data = readmatrix(filepath, 'NumHeaderLines', 1);
    end

    % 检查数据列数是否与标准列名匹配
    num_cols = size(data, 2);
    num_standard = length(standard_col_names);

    if num_cols < num_standard
        error('数据只有%d列，但标准列名需要%d列', num_cols, num_standard);
    elseif num_cols > num_standard
        fprintf('警告: 数据有%d列，标准列名只有%d列，多余列将被忽略\n', num_cols, num_standard);
        data = data(:, 1:num_standard);  % 只取前N列
    end

    fprintf('数据列数: %d\n', size(data, 2));
    fprintf('标准列名: ');
    fprintf('%s ', standard_col_names{:});
    fprintf('\n');
    fprintf('running_success_flag 在第 %d 列\n', flag_col);
    fprintf('time 在第 %d 列\n', time_col);
    fprintf('总采样点数: %d\n', size(data, 1));

    %% ========== 3. 创建导出目录 ==========
    [file_dir, file_name, ~] = fileparts(filepath);
    if isempty(file_dir)
        file_dir = '.';
    end

    export_dir = fullfile(file_dir, file_name);
    if ~isfolder(export_dir)
        mkdir(export_dir);
        fprintf('创建导出目录: %s\n', export_dir);
    end

    %% ========== 4. 找到 running_success_flag == 1 的连续段 ==========
    flag = data(:, flag_col);
    valid = (flag == 1);

    diff_valid = diff([0; valid; 0]);
    seg_start = find(diff_valid == 1);
    seg_end = find(diff_valid == -1) - 1;

    num_segments = length(seg_start);
    fprintf('找到 %d 个有效数据段\n', num_segments);

    if num_segments == 0
        fprintf('没有 running_success_flag = 1 的数据段，无需导出\n');
        return;
    end

    %% ========== 5. 逐段导出（始终使用标准列名） ==========
    for i = 1:num_segments
        start_idx = seg_start(i);
        end_idx = seg_end(i);

        segment = data(start_idx:end_idx, :);
        seg_time = segment(:, time_col);
        duration = seg_time(end) - seg_time(1);

        fprintf('  段 %d: 索引[%d:%d], 持续时间%.3f秒, 采样点数%d\n', ...
            i, start_idx, end_idx, duration, size(segment, 1));

        % 导出文件名: .\文件名\001.csv, .\文件名\002.csv, ...
        filename = sprintf('%03d.csv', i);
        filepath_out = fullfile(export_dir, filename);

        % 写入CSV（始终使用标准列名，不是原始列名）
        T = array2table(segment, 'VariableNames', standard_col_names);
        writetable(T, filepath_out);

        fprintf('    -> 导出: %s\n', filepath_out);
    end

    fprintf('完成！共导出 %d 个文件到 %s\n', num_segments, export_dir);
end