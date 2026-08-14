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
#include "zephyr/drivers/sensor.h"
#include "ano/ano_base.h"
#include "ano/ano_device/ano_device_com.h"
#include "menu/menu.h"
#include "ano.h"

static const struct device *mpu6050_pst = DEVICE_DT_GET(DT_NODELABEL(mpu6050));


int main(void)
{
	struct sensor_value accel[3],gyro[3],temp;

	while (1) {
		sensor_sample_fetch(mpu6050_pst);
		sensor_channel_get(mpu6050_pst, SENSOR_CHAN_ACCEL_XYZ, accel);
		sensor_channel_get(mpu6050_pst, SENSOR_CHAN_GYRO_XYZ,gyro);
		sensor_channel_get(mpu6050_pst, SENSOR_CHAN_DIE_TEMP, &temp);

		ano_check_data(g_com_ano_pst);
		com_check_to_send();
		// menu_task_v();
		k_sleep(K_MSEC(30));
	}
	return 0;
}

