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
#include "menu.h"
#include "mess/value_to_str.h"
#include <stdio.h>
#include <zephyr/device.h>
#include <zephyr/display/cfb.h>
#include <mpu6050/mpu6050.h>

/* OLED 设备 (与 menu.c 共用同一个) */
static const struct device *s_oled_pst =
    DEVICE_DT_GET(DT_NODELABEL(ssd1306));

/* ================================================================
 * 叶子 draw 函数
 * ================================================================ */

static void s_draw_hello(struct menu_node_t *self)
{
    cfb_framebuffer_set_font(s_oled_pst, 0);
    cfb_print(s_oled_pst, self->base.name, 0, 0);
    cfb_print(s_oled_pst, "Hello World!", 0, 20);
}

static void s_draw_value(struct menu_node_t *self)
{
    cfb_framebuffer_set_font(s_oled_pst, 0);
    cfb_print(s_oled_pst, self->base.name, 0, 0);
    /* 底部值 + 编辑状态由 menu_task_v 统一绘制 */
}

static void s_draw_mpu6050(struct menu_node_t* self)
{
    char num[16];   /* 放数值 */
    char buf[32];   /* 放最终结果 */
    cfb_framebuffer_set_font(s_oled_pst, 0);
    cfb_print(s_oled_pst, self->base.name, 0, 0);
    struct mpu6050_vec3_t mpu6050_vec3_st = {0};
    mpu6050_get_accel(&mpu6050_vec3_st);

    float_to_str(num, sizeof(num), sensor_value_to_float(&mpu6050_vec3_st.x_st), 2);
    snprintf(buf, sizeof(buf), "ax:%s", num);   /* ✅ num 和 buf 分开 */
    cfb_print(s_oled_pst, buf, 0, 10);

    /* ay */
    float_to_str(num, sizeof(num), sensor_value_to_float(&mpu6050_vec3_st.y_st), 2);
    snprintf(buf, sizeof(buf), "ay:%s", num);   /* ✅ num 和 buf 分开 */
    cfb_print(s_oled_pst, buf, 64, 10);

    /* az */
    float_to_str(num, sizeof(num), sensor_value_to_float(&mpu6050_vec3_st.z_st), 2);
    snprintf(buf, sizeof(buf), "az:%s", num);   /* ✅ num 和 buf 分开 */
    cfb_print(s_oled_pst, buf, 0, 20);

    mpu6050_get_gyro(&mpu6050_vec3_st);
    
    float_to_str(num, sizeof(num), sensor_value_to_float(&mpu6050_vec3_st.x_st), 2);
    snprintf(buf, sizeof(buf), "gx:%s", num);
    cfb_print(s_oled_pst, buf, 64, 20);

    /* gy */
    float_to_str(num, sizeof(num), sensor_value_to_float(&mpu6050_vec3_st.y_st), 2);
    snprintf(buf, sizeof(buf), "gy:%s", num);
    cfb_print(s_oled_pst, buf, 0, 30);

    /* gz */
    float_to_str(num, sizeof(num), sensor_value_to_float(&mpu6050_vec3_st.z_st), 2);
    snprintf(buf, sizeof(buf), "gz:%s", num);
    cfb_print(s_oled_pst, buf, 64, 30);

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

static struct menu_node_t g_mpu6050_raw_st;

struct menu_base_t* g_mpu6050_raw_oled_pst;

/* ================================================================
 * 构建菜单树
 * ================================================================ */

static int s_build_menu_tree(void)
{
    Create_Menu_Folder(NULL, g_root, "Main");
    Create_Menu_Folder(&g_root, g_menu_test, "Test");
    Create_Menu_Leaf(&g_menu_test, g_item_hello, "Hello", s_draw_hello);

    g_mpu6050_raw_oled_pst = Create_Menu_Leaf(&g_root,g_mpu6050_raw_st, "mpu6050", s_draw_mpu6050);

    Create_Menu_Leaf_Range(&g_menu_test, g_item_value,
                           "Value", s_draw_value,
                           &s_test_value, 0, 100);
    menu_set_default_int(&g_item_value, 50);

    return 0;
}

/* ================================================================
 * 初始化 (menu_init_v 已由 menu.c 处理动作注册)
 * ================================================================ */

// void menu_oled_init_v(void)
// {
//     s_build_menu_tree();
// }
SYS_INIT(s_build_menu_tree, APPLICATION, 10);

