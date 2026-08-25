function filter_raw_super_max(filepath)
% FILTER_RAW_SUPER 读取CSV文件，根据running_success_flag=1拆分数据段，分别导出CSV
% 导出的时间从0开始，并四舍五入到合理精度
%
% 输入:
%   filepath - CSV文件路径
%
% 示例:
%   filter_raw_super('data.csv')

    %% ========== 预设标准列名和列顺序 ==========
    standard_col_names = {'time', 'pwma', 'pwmb', 'pitch', 'motor_a_speed', ...
                          'motor_b_speed', 'target_speed', 'target_angle', 'running_success_flag'};

    flag_col = 9;
    time_col = 1;
    TIME_PRECISION = 6;  % 时间保留6位小数

    %% ========== 1. 读取CSV文件 ==========
    if ~isfile(filepath)
        error('File not found: %s', filepath);
    end

    fid = fopen(filepath, 'r');
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

    %% ========== 2. 读取数据 ==========
    if is_numeric
        fprintf('No header detected, using standard column names\n');
        data = readmatrix(filepath);
    else
        fprintf('Header detected, skipping header line\n');
        data = readmatrix(filepath, 'NumHeaderLines', 1);
    end

    num_cols = size(data, 2);
    num_standard = length(standard_col_names);

    if num_cols < num_standard
        error('Data has %d columns, but %d columns required', num_cols, num_standard);
    elseif num_cols > num_standard
        fprintf('Warning: Data has %d columns, using first %d columns\n', num_cols, num_standard);
        data = data(:, 1:num_standard);
    end

    fprintf('Columns: %d, Samples: %d\n', size(data, 2), size(data, 1));

    %% ========== 3. 创建导出目录 ==========
    [file_dir, file_name, ~] = fileparts(filepath);
    if isempty(file_dir)
        file_dir = '.';
    end

    export_dir = fullfile(file_dir, file_name);
    if ~isfolder(export_dir)
        mkdir(export_dir);
        fprintf('Export dir: %s\n', export_dir);
    end

    %% ========== 4. 找到 running_success_flag == 1 的连续段 ==========
    flag = data(:, flag_col);
    valid = (flag == 1);

    diff_valid = diff([0; valid; 0]);
    seg_start = find(diff_valid == 1);
    seg_end = find(diff_valid == -1) - 1;

    num_segments = length(seg_start);
    fprintf('Found %d valid segments\n', num_segments);

    if num_segments == 0
        fprintf('No running_success_flag = 1 segments found\n');
        return;
    end

    %% ========== 5. 逐段导出（时间从0开始，四舍五入） ==========
    for i = 1:num_segments
        start_idx = seg_start(i);
        end_idx = seg_end(i);

        segment = data(start_idx:end_idx, :);

        % 时间归一化：减去该段初始时间
        t0 = segment(1, time_col);
        segment(:, time_col) = segment(:, time_col) - t0;

        % 四舍五入消除浮点误差
        segment(:, time_col) = round(segment(:, time_col), TIME_PRECISION);

        seg_time = segment(:, time_col);
        duration = seg_time(end) - seg_time(1);

        fprintf('  Seg %d: idx[%d:%d], duration %.3fs, samples %d\n', ...
            i, start_idx, end_idx, duration, size(segment, 1));

        filename = sprintf('%03d.csv', i);
        filepath_out = fullfile(export_dir, filename);

        T = array2table(segment, 'VariableNames', standard_col_names);
        writetable(T, filepath_out);

        fprintf('    -> Exported: %s\n', filepath_out);
    end

    fprintf('Done! Exported %d files to %s\n', num_segments, export_dir);
end