/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdbool.h>
#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/display/cfb.h>
#include <zephyr/sys/printk.h>

#include "menu/menu.h"
#include "menu/menu_input.h"
#include "menu/OLED_Menu.h"

static const struct device *s_oled_pst = DEVICE_DT_GET(DT_NODELABEL(ssd1306));

int main(void)
{
	if (!device_is_ready(s_oled_pst)) {
		printk("SSD1306 is not ready\n");
	}

	cfb_framebuffer_init(s_oled_pst);
	cfb_framebuffer_clear(s_oled_pst, true);
	cfb_framebuffer_set_font(s_oled_pst, 0);
	cfb_print(s_oled_pst, "hello zephyr", 0, 0);
	cfb_framebuffer_finalize(s_oled_pst);

	/* 菜单初始化: 注册动作处理器 + 构建菜单树 */
	menu_init_v();
	menu_oled_init_v();

	while (1) {
		/* 菜单显示 (脏标记驱动, 无变化时不刷新) */
		menu_task_v();

		/* 长按检测 (每 30ms 查询一次) */
		menu_input_poll();

		k_sleep(K_MSEC(30));
	}
	return 0;
}
