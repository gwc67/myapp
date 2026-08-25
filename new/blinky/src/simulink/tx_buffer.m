addpath('D:\Downloads\zephyr_new\myapp\new\blinky\src\simulink\balance_wave\fillter_function');
addpath('D:\Downloads\zephyr_new\myapp\new\blinky\src\simulink\balance_wave\analyze');

%filter_raw_super('D:\Downloads\zephyr_new\myapp\new\blinky\src\simulink\balance_wave\balance_wave_raw\test4.csv');
%filter_raw_super_max('D:\Downloads\zephyr_new\myapp\new\blinky\src\simulink\balance_wave\balance_wave_raw\test4.csv');
analyze_segments('D:\Downloads\zephyr_new\myapp\new\blinky\src\simulink\balance_wave\balance_wave_raw\test4');
analyze_target_speed_intervals("D:\Downloads\zephyr_new\myapp\new\blinky\src\simulink\balance_wave\balance_wave_raw\test4");