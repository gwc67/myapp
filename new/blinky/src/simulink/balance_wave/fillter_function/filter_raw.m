function filter_raw(filepath)
% FILTER_RAW 读取CSV文件，根据running_success_flag=1拆分数据段，分别导出CSV
%
% 输入:
%   filepath - CSV文件路径，例如 'data.csv'
%
% 输出:
%   无返回值，直接在CSV所在目录下创建子文件夹，导出拆分后的文件
%
% 示例:
%   filter_raw('D:\\balance_car\\log.csv')
%   filter_raw('log_20250825.csv')

    %% 1. 读取CSV文件
    if ~isfile(filepath)
        error('文件不存在: %s', filepath);
    end

    % 读取表头
    fid = fopen(filepath, 'r');
    header_line = fgetl(fid);
    fclose(fid);

    col_names = strsplit(header_line, ',');
    col_names = strtrim(col_names);  % 去除首尾空格

    % 读取数据（跳过表头）
    data = readmatrix(filepath, 'NumHeaderLines', 1);

    fprintf('读取文件: %s\n', filepath);
    fprintf('列名: ');
    fprintf('%s ', col_names{:});
    fprintf('\n总采样点数: %d\n', size(data, 1));

    %% 2. 自动匹配列名，找到关键列
    flag_col = find(strcmpi(col_names, 'running_success_flag'));
    time_col = find(strcmpi(col_names, 'time'));

    if isempty(flag_col)
        error('未找到列名为 running_success_flag 的列');
    end
    if isempty(time_col)
        time_col = 1;  % 默认第一列为时间
        fprintf('警告: 未找到time列，默认使用第1列\n');
    end

    fprintf('匹配到 running_success_flag 在第 %d 列\n', flag_col);
    fprintf('匹配到 time 在第 %d 列\n', time_col);

    %% 3. 创建导出目录
    [file_dir, file_name, ~] = fileparts(filepath);
    if isempty(file_dir)
        file_dir = '.';
    end

    export_dir = fullfile(file_dir, file_name);
    if ~isfolder(export_dir)
        mkdir(export_dir);
        fprintf('创建导出目录: %s\n', export_dir);
    end

    %% 4. 找到 running_success_flag == 1 的连续段
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

    %% 5. 逐段导出
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

        % 写入CSV（带表头）
        T = array2table(segment, 'VariableNames', col_names);
        writetable(T, filepath_out);

        fprintf('    -> 导出: %s\n', filepath_out);
    end

    fprintf('完成！共导出 %d 个文件到 %s\n', num_segments, export_dir);
end