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
#include "ano/ano_base.h"
#include "ano/ano_device/ano_device_com.h"
#include "menu/menu.h"
#include "ano.h"
#include "mpu6050/ahrs_madgwick.h"
#include "mpu6050/euler.h"


int main(void)
{


	while (1) {
		euler_update();
		ano_check_data(g_com_ano_pst);
		com_check_to_send();
		menu_task_v();
		k_sleep(K_MSEC(30));
	}
	return 0;
}

