extern struct ano_base_t g_com_ano_pst;
在D:\Downloads\zephyr_new\myapp\new\blinky\src\ano\ano.h 中，不需要在uart_transport.c 中声明
D:\Downloads\zephyr_new\myapp\new\blinky\src\ano\ano_device\uart_transport.c 应该在D:\Downloads\zephyr_new\myapp\new\blinky\src\uart 中，

D:\Downloads\zephyr_new\myapp\new\blinky\src\menu 下的内容在uart的任务下不要更改，我已经配置好了，不需要额外在主循环初始化

D:\Downloads\zephyr_new\myapp\new\blinky\src\ano\ano_device\uart_transport.c 下的设计哲学是不是有问题，
为什么IT模式和DMA模式是互斥的，如果我有其他uart使用的DMA，有其他设备使用的是IT，岂不是矛盾