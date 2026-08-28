function analyze_e_ec(csv_path)
% ANALYZE_E_EC 分析单个CSV文件中 e 和 ec 列的范围、最值及分布
%
% 输入:
%   csv_path - filter_raw_batch_v5 生成的 CSV 文件路径
%
% 示例:
%   analyze_e_ec('D:\data\filter\tar0\001_tar0.csv')
%   analyze_e_ec('001_tar0.csv')  % 当前目录下

    %% ========== 1. 读取数据 ==========
    if ~isfile(csv_path)
        error('文件不存在: %s', csv_path);
    end

    T = readtable(csv_path);
    
    % 检查必需列
    required_cols = {'e', 'ec'};
    for i = 1:length(required_cols)
        if ~ismember(required_cols{i}, T.Properties.VariableNames)
            error('CSV缺少必需列: %s', required_cols{i});
        end
    end

    e = T.e;
    ec = T.ec;
    n = length(e);

    %% ========== 2. 计算统计量 ==========
    e_min = min(e);     e_max = max(e);
    ec_min = min(ec);   ec_max = max(ec);
    e_mean = mean(e);   ec_mean = mean(ec);
    e_std = std(e);     ec_std = std(ec);
    e_abs_max = max(abs(e));
    ec_abs_max = max(abs(ec));

    %% ========== 3. 打印统计结果 ==========
    fprintf('\n');
    fprintf('╔══════════════════════════════════════════════════════╗\n');
    fprintf('║           e & ec 数据分析报告                        ║\n');
    fprintf('╠══════════════════════════════════════════════════════╣\n');
    fprintf('║ 文件: %s\n', csv_path);
    fprintf('║ 样本数: %d\n', n);
    fprintf('╠══════════════════════════════════════════════════════╣\n');
    fprintf('║ %-10s │ %-14s │ %-14s ║\n', '统计量', 'e (误差)', 'ec (误差变化率)');
    fprintf('╠════════════╪════════════════╪══════════════════════╣\n');
    fprintf('║ %-10s │ %14.4f │ %18.4f ║\n', '最小值', e_min, ec_min);
    fprintf('║ %-10s │ %14.4f │ %18.4f ║\n', '最大值', e_max, ec_max);
    fprintf('║ %-10s │ %14.4f │ %18.4f ║\n', '均值', e_mean, ec_mean);
    fprintf('║ %-10s │ %14.4f │ %18.4f ║\n', '标准差', e_std, ec_std);
    fprintf('║ %-10s │ %14.4f │ %18.4f ║\n', '最大绝对值', e_abs_max, ec_abs_max);
    fprintf('╚════════════╧════════════════╧══════════════════════╝\n');

    %% ========== 4. 可视化 ==========
    figure('Name', sprintf('e & ec 分析 - %s', csv_path), ...
           'Position', [100 100 1400 450], ...
           'Color', 'white');

    % 1. e 的时间序列
    subplot(1, 3, 1);
    plot(e, 'b-', 'LineWidth', 0.5);
    hold on;
    yline(0, 'r--', 'LineWidth', 1);
    yline(e_min, 'g:', 'LineWidth', 1.5);
    yline(e_max, 'g:', 'LineWidth', 1.5);
    title('e (误差) 时序', 'FontSize', 12, 'FontWeight', 'bold');
    ylabel('e'); xlabel('样本点');
    grid on;
    legend('e', '0', 'Min/Max', 'Location', 'best');

    % 2. ec 的时间序列
    subplot(1, 3, 2);
    plot(ec, 'b-', 'LineWidth', 0.5);
    hold on;
    yline(0, 'r--', 'LineWidth', 1);
    yline(ec_min, 'g:', 'LineWidth', 1.5);
    yline(ec_max, 'g:', 'LineWidth', 1.5);
    title('ec (误差变化率) 时序', 'FontSize', 12, 'FontWeight', 'bold');
    ylabel('ec'); xlabel('样本点');
    grid on;
    legend('ec', '0', 'Min/Max', 'Location', 'best');

    % 3. e-ec 散点图（模糊PID设计关键！）
    subplot(1, 3, 3);
    scatter(e, ec, 3, 'b', 'filled', 'MarkerFaceAlpha', 0.3);
    hold on;
    
    % 画坐标轴
    xline(0, 'r--', 'LineWidth', 1);
    yline(0, 'r--', 'LineWidth', 1);
    
    % 画边界框
    rectangle('Position', [e_min, ec_min, e_max-e_min, ec_max-ec_min], ...
              'EdgeColor', 'g', 'LineStyle', '--', 'LineWidth', 1.5);
    
    xlabel('e', 'FontSize', 11);
    ylabel('ec', 'FontSize', 11);
    title('e-ec 相平面', 'FontSize', 12, 'FontWeight', 'bold');
    grid on;
    axis equal;
    
    % 范围标注
    text(e_max*0.7, ec_max*0.9, ...
         sprintf('e ∈ [%.2f, %.2f]\nec ∈ [%.2f, %.2f]\n|e|_{max} = %.2f\n|ec|_{max} = %.2f', ...
                 e_min, e_max, ec_min, ec_max, e_abs_max, ec_abs_max), ...
         'BackgroundColor', [1 1 1 0.8], ...
         'EdgeColor', 'k', ...
         'FontSize', 9, ...
         'HorizontalAlignment', 'center');

    %% ========== 5. 返回统计结果（可选）==========
    % 如果想在命令行用变量接收结果，取消下面注释
    % stats.e_min = e_min; stats.e_max = e_max;
    % stats.ec_min = ec_min; stats.ec_max = ec_max;
    % stats.e_abs_max = e_abs_max; stats.ec_abs_max = ec_abs_max;
    % assignin('base', 'stats', stats);

end