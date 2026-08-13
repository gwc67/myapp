/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdbool.h>
#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/display/cfb.h>
#include <zephyr/sys/printk.h>
#include "menu/menu.h"



int main(void)
{
	while (1) {
		menu_task_v();
		k_sleep(K_MSEC(30));
	}
	return 0;
}

