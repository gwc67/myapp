/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/display/cfb.h>
#include <zephyr/sys/printk.h>

#include "menu/menu.h"
#include "zephyr/drivers/uart.h"
#include "zephyr/sys/ring_buffer.h"
#include "zephyr/sys/time_units.h"

#define  UART_NODE DT_NODELABEL(usart2)

static const struct device* uart_dev_pst = DEVICE_DT_GET(UART_NODE);

//环形缓冲区大小
#define RX_BUF_SIZE 128
#define TX_BUF_SIZE	128


static uint8_t rx_ring_buf_puc[RX_BUF_SIZE];
static uint8_t tx_ring_buf_puc[TX_BUF_SIZE];

static struct ring_buf rx_ring;
static struct ring_buf tx_ring;

static void uart_callback(const struct device *dev,struct uart_event * evt,void *user_data);
static void send_next_data(void);

int main(void)
{


	ring_buf_init(&rx_ring, RX_BUF_SIZE, rx_ring_buf_puc);
	ring_buf_init(&tx_ring, TX_BUF_SIZE, tx_ring_buf_puc);
	
	int ret = uart_callback_set(uart_dev_pst, uart_callback, NULL);

	if (ret < 0) {
		return  ret;
	}
	
	ret = uart_rx_enable(uart_dev_pst, NULL, 0, SYS_FOREVER_US);  //接受持续开启
	if (ret < 0) {
	return ret;
	}
	
	while (1) {
		/* 菜单显示 (脏标记驱动, 无变化时不刷新) */
		uint8_t ch;

		while (ring_buf_get(&rx_ring, &ch, 1) == 1) {
			ring_buf_put(&tx_ring,&ch,1);
			send_next_data();
		}
		
		menu_task_v();
		k_sleep(K_MSEC(30));
	}
	return 0;
}

static void send_next_data(void)
{
	uint8_t ch;
	if (ring_buf_get(&tx_ring,&ch,1) == 1) {
		uart_tx(uart_dev_pst,&ch,1,SYS_FOREVER_US);
	}
}


static void uart_callback(const struct device * dev ,struct uart_event* evt,void *user_data)
{
	switch (evt->type) {
		case UART_RX_RDY:
			ring_buf_put(&rx_ring,evt->data.rx.buf,evt->data.rx.len);
			break;
		case UART_RX_STOPPED:
			break;
		default:
			break;
	}	
}

