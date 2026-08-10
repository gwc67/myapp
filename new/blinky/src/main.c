/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "zephyr/sys/printk.h"
#include "zephyr/syscalls/device.h"
#include <stdbool.h>
#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>
#include <zephyr/display/cfb.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct device *display = DEVICE_DT_GET(DT_NODELABEL(ssd1306));


int main(void)
{
	if(!device_is_ready(display))
	{
		printk("SSD1306 is not ready\n");
	}

	cfb_framebuffer_init(display);
	cfb_framebuffer_clear(display, true);
	cfb_framebuffer_invert(display);
	cfb_print(display, "hello zephyr", 0, 0);
	cfb_framebuffer_finalize(display);
	while (1) {

	}
	return 0;
}
