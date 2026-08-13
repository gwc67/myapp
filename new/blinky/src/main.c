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
#include "my_ring/my_ring.h"
#include "uart/uart_base.h"
#include "uart/uarts.h"



int main(void)
{
	uint8_t test_puc[10] = {0};
	while (1) {
		// uart_transmit(g_uart2_pst, test_puc, sizeof(test_puc));

		uint32_t actual =  my_ring_buf_get(g_ut3_rx_ring_pst, test_puc, sizeof(test_puc));	
		uart_transmit(g_uart3_pst, test_puc, actual);
		
		menu_task_v();
		k_sleep(K_MSEC(30));
	}
	return 0;
}

