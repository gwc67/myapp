/*
 * OLED 菜单页面定义
 *
 * 菜单树结构 (在 OLED_Menu.c 中构建):
 *
 *   g_root (Main)          ← 文件夹: 自动显示子项列表
 *   └─ g_menu_test         ← 文件夹
 *      ├─ g_item_hello     ← 叶子: draw 显示 hello
 *      └─ g_item_value     ← 可编辑 int32 (0~100)
 */

#ifndef OLED_MENU_H
#define OLED_MENU_H

#include "menu.h"

/* 根节点 */
extern struct menu_node_t g_root;

/* 初始化: 构建菜单树 */
void menu_oled_init_v(void);

#endif /* OLED_MENU_H */
