# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.28.0)
set(BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(blinky)

target_sources(app PRIVATE
    src/main.c
    src/menu/menu_input.c
    src/menu/menu.c
    src/menu/OLED_Menu.c
    src/menu/font.c
    src/menu/ano_true.c
    src/menu/ano_base.c
    src/menu/ano_board_init.c
)

target_include_directories(app PRIVATE
    src
    src/menu
    src/ano
    src/ano/ano_device
)