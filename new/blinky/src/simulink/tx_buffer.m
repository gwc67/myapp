addpath('D:\Downloads\zephyr_new\myapp\new\blinky\src\simulink\balance_wave\fillter_function');
addpath('D:\Downloads\zephyr_new\myapp\new\blinky\src\simulink\balance_wave\analyze');

%filter_raw_super('D:\Downloads\zephyr_new\myapp\new\blinky\src\simulink\balance_wave\balance_wave_raw\test4.csv');
% filter_raw_batch('D:\Downloads\zephyr_new\myapp\new\blinky\src\simulink\balance_wave\balance_wave_raw\bal_kp_75_bal_kd_180');
% analyze_segments('D:\Downloads\zephyr_new\myapp\new\blinky\src\simulink\balance_wave\balance_wave_raw\bal_kp_75_bal_kd_180_filter');
%analyze_target_speed_intervals("D:\Downloads\zephyr_new\myapp\new\blinky\src\simulink\balance_wave\balance_wave_raw\test4");
compare_groups_6('D:\Downloads\zephyr_new\myapp\new\blinky\src\simulink\balance_wave\balance_wave_raw\bal_kp_75_bal_kd_180_filter');