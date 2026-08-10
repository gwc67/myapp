/*
 * OLED 菜单树定义 (Zephyr 移植版)
 *
 * 树结构:
 *   g_root (Main)
 *   └─ [Test]
 *      ├─ Hello        ← 叶子: 显示 "Hello World!"
 *      └─ Value        ← 可编辑 int32 (0~100, 步进切换)
 */

#include "OLED_Menu.h"

#include <zephyr/device.h>
#include <zephyr/display/cfb.h>

/* OLED 设备 (与 menu.c 共用同一个) */
static const struct device *s_display =
    DEVICE_DT_GET(DT_NODELABEL(ssd1306));

/* ================================================================
 * 叶子 draw 函数
 * ================================================================ */

static void s_draw_hello(struct menu_node_t *self)
{
    cfb_framebuffer_set_font(s_display, 0);
    cfb_print(s_display, self->base.name, 0, 0);
    cfb_print(s_display, "Hello World!", 0, 20);
}

static void s_draw_value(struct menu_node_t *self)
{
    cfb_framebuffer_set_font(s_display, 0);
    cfb_print(s_display, self->base.name, 0, 0);
    /* 底部值 + 编辑状态由 menu_task_v 统一绘制 */
}

/* ================================================================
 * 可编辑数据
 * ================================================================ */

static int32_t s_test_value = 50;

/* ================================================================
 * 节点实例
 * ================================================================ */

struct menu_node_t g_root;
static struct menu_node_t g_menu_test;
static struct menu_node_t g_item_hello;
static struct menu_node_t g_item_value;

/* ================================================================
 * 构建菜单树
 * ================================================================ */

static void s_build_menu_tree(void)
{
    Create_Menu_Folder(NULL, g_root, "Main");
    Create_Menu_Folder(&g_root, g_menu_test, "Test");
    Create_Menu_Leaf(&g_menu_test, g_item_hello, "Hello", s_draw_hello);

    Create_Menu_Leaf_Range(&g_menu_test, g_item_value,
                           "Value", s_draw_value,
                           &s_test_value, 0, 100);
    menu_set_default_int(&g_item_value, 50);
}

/* ================================================================
 * 初始化 (menu_init_v 已由 menu.c 处理动作注册)
 * ================================================================ */

void menu_oled_init_v(void)
{
    s_build_menu_tree();
}
