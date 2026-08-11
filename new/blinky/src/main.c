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

#define UART_NODE DT_NODELABEL(usart2)

static const struct device *uart_dev_pst = DEVICE_DT_GET(UART_NODE);

/* 环形缓冲区 */
#define RX_BUF_SIZE 128
#define TX_BUF_SIZE 128

static uint8_t rx_ring_buf_puc[RX_BUF_SIZE];
static uint8_t tx_ring_buf_puc[TX_BUF_SIZE];

static struct ring_buf rx_ring;
static struct ring_buf tx_ring;

static void uart_isr(const struct device *dev, void *user_data);
static void send_next_data(void);

int main(void)
{
	ring_buf_init(&rx_ring, RX_BUF_SIZE, rx_ring_buf_puc);
	ring_buf_init(&tx_ring, TX_BUF_SIZE, tx_ring_buf_puc);

	if (!device_is_ready(uart_dev_pst)) {
		printk("UART device not ready\n");
		return -1;
	}

	/* ── IT 中断驱动模式 ── */
	/* 1. 注册 ISR 回调 (不是 uart_callback_set!) */
	uart_irq_callback_user_data_set(uart_dev_pst, uart_isr, NULL);

	/* 2. 使能 RX 中断 (不是 uart_rx_enable!) */
	uart_irq_rx_enable(uart_dev_pst);

	printk("UART IT mode started, waiting for data...\n");

	while (1) {
		uint8_t ch;

		/* 从 RX ring_buf 取出字节 → 回显到 TX */
		while (ring_buf_get(&rx_ring, &ch, 1) == 1) {
			ring_buf_put(&tx_ring, &ch, 1);
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
	if (ring_buf_get(&tx_ring, &ch, 1) == 1) {
		/* IT 模式发送用 polling，ISR 会处理 TX 中断 */
		uart_poll_out(uart_dev_pst, ch);
	}
}

/* ── IT 中断驱动 ISR ──
 * 每收到一个字节硬件就触发一次中断
 * uart_irq_update() 读取硬件状态寄存器
 * uart_irq_rx_ready() 检查 RX 是否有数据
 * uart_fifo_read() 从硬件 FIFO/DR 读取字节
 */
static void uart_isr(const struct device *dev, void *user_data)
{
	uart_irq_update(dev);

	/* RX: 逐字节读出放入 ring_buf */
	while (uart_irq_rx_ready(dev)) {
		uint8_t ch;
		if (uart_fifo_read(dev, &ch, 1) == 1) {
			ring_buf_put(&rx_ring, &ch, 1);
		}
	}
}
