# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.28.0)
set(BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# 直接 cmake 构建 (不走 west) 时模块系统不会自动加载 hal_stm32,
# 导致 board dts 里的 #include <st/f4/stm32f407z(e-g)tx-pinctrl.dtsi> 解析不到.
# 这里手动把 hal_stm32 模块的 dts 根注册进 DTS_ROOT, 让 pinctrl 定义可见.
list(APPEND DTS_ROOT "$ENV{ZEPHYR_BASE}/../modules/hal/stm32")

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(blinky)

target_sources(app PRIVATE
    src/main.c
    src/menu/menu_input.c
    src/menu/menu.c
    src/menu/OLED_Menu.c
    src/menu/font.c
    src/ano/ano_true.c
    src/ano/ano_base.c
    src/ano/ano_board_init.c
    src/uart/uart_base.c
    src/uart/uart_it.c
    src/uart/uart_dma.c
    src/uart/uart_board_init.c
    src/my_ring/my_ring.c
)

target_include_directories(app PRIVATE
    src
    src/menu
    src/ano
    src/uart
    src/my_ring
)