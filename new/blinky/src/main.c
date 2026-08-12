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
#include "zephyr/syscalls/uart.h"

#define UART_NODE DT_NODELABEL(usart2)

static const struct device *uart_dev_pst = DEVICE_DT_GET(UART_NODE);

/* 环形缓冲区 */
#define RX_BUF_SIZE 128
#define TX_BUF_SIZE 128

/* RX 双缓冲轮转 */
static uint8_t rx_buf_a[RX_BUF_SIZE];
// static uint8_t rx_buf_b[RX_BUF_SIZE];

/* TX 线性缓冲 DMA需要连续内存，这个决定DMA发送下，无法使用tx环形缓冲区 */
static uint8_t tx_buf[TX_BUF_SIZE];
static volatile bool tx_busy;  //DMA正在发送标志位


/* ISR -> main 传递 */
static uint8_t rx_ring_buf_puc[RX_BUF_SIZE];
static struct ring_buf rx_ring;

//异步回调函数
static void uart_async_cb(const struct device* dev,struct uart_event* evt,void *user_data)
{
	switch (evt->type) {
		case UART_RX_RDY:
			/* data.rx.buf + evt.data.rx.offset 指向有效数据的起始 */
			/* evt->data.rx.len 是本批有效长度 */
			ring_buf_put(&rx_ring, &evt->data.rx.buf[evt->data.rx.offset], evt->data.rx.len);
			break;
			//当前缓冲区已满，启动下一个缓冲区
		case UART_RX_BUF_RELEASED:
			uart_rx_buf_rsp(dev, evt->data.rx_buf.buf, RX_BUF_SIZE);
			break;
		case UART_RX_DISABLED:
			/* 缓冲区写满 / 出错时驱动会停掉 DMA 接收.
			 * 单缓冲方案必须在这里重新使能, 否则收一次 128 字节就永久停了.
			 * (该事件在线程/工作队列上下文发出, 这里可以直接调用)
			 */
			uart_rx_enable(uart_dev_pst, rx_buf_a, RX_BUF_SIZE, 1000);
			break;
		case UART_TX_DONE:
			tx_busy = false;
			break;
			//中断被中止
		case UART_TX_ABORTED:
			tx_busy = false;
			printk("TX aborted %d bytes\n",evt->data.tx.len);
		default:
			break;
	}
}

// static void uart_isr(const struct device *dev, void *user_data);
// static void send_next_data(void);

int main(void)
{
	ring_buf_init(&rx_ring, RX_BUF_SIZE, rx_ring_buf_puc);


	if (!device_is_ready(uart_dev_pst)) {
		// printk("UART device not ready\n");
		return -1;
	}


	int ret = uart_callback_set(uart_dev_pst, uart_async_cb, NULL);

	if (ret ) {
		// printk("uart_callback_failed: %d\r\n",ret);
		return ret;
	}

	/* timeout 单位是微秒, 是"行空闲多久算一包"的判定时间.
	 * 不能传 SYS_FOREVER_US(-1) !!
	 * 传 -1 时 STM32 驱动的 IDLE 中断不会触发 dma_rx_flush,
	 * 缓冲区永远不生成 UART_RX_RDY 事件, 数据进 DMA 但取不出来.
	 * 传一个具体值(如 1000us), IDLE 后 1ms 无新数据就会 flush 并上报.
	 */
	ret = uart_rx_enable(uart_dev_pst, rx_buf_a, RX_BUF_SIZE, 1000);

	if (ret ) {
		// printk("uart_callback_failed: %d\r\n",ret);
		return ret;
	}
	
	/* ── IT 中断驱动模式 ── */
	/* 1. 注册 ISR 回调 (不是 uart_callback_set!) */
	// uart_irq_callback_user_data_set(uart_dev_pst, uart_isr, NULL);

	/* 2. 使能 RX 中断 (不是 uart_rx_enable!) */
	// uart_irq_rx_enable(uart_dev_pst);
	// 使能tx中断

	// uart_irq_tx_enable(uart_dev_pst);

	// printk("UART IT mode started, waiting for data...\n");

	while (1) {
		uint8_t ch;

		/* 从 RX ring_buf 取出字节 → 回显到 TX */
		while (ring_buf_get(&rx_ring, &ch, 1) == 1) {
			if (!tx_busy) {
				tx_buf[0] = ch;
				tx_busy = true;
				uart_tx(uart_dev_pst,tx_buf,1,SYS_FOREVER_US);
			}
		}

		menu_task_v();
		k_sleep(K_MSEC(30));
	}
	return 0;
}

// static void send_next_data(void)
// {
// 	 uint8_t ch;

//     // 尝试填充 FIFO
//     while (ring_buf_peek(&tx_ring, &ch, 1) == 1) {
//         if (uart_fifo_fill(uart_dev_pst, &ch, 1) == 1) {
//             ring_buf_get(&tx_ring, &ch, 1); // 成功才消费
//         } else {
//             break; // FIFO 满，等下次 TX 中断
//         }
//     }

// 	if (ring_buf_is_empty(&tx_ring)) {
//         uart_irq_tx_disable(uart_dev_pst);   // 没数据了，关中断防止空转
//     } else {
//         uart_irq_tx_enable(uart_dev_pst);    // 还有数据但 FIFO 满，保持中断等待排水
//     }
// }

// /* ── IT 中断驱动 ISR ──
//  * 每收到一个字节硬件就触发一次中断
//  * uart_irq_update() 读取硬件状态寄存器
//  * uart_irq_rx_ready() 检查 RX 是否有数据
//  * uart_fifo_read() 从硬件 FIFO/DR 读取字节
//  */
// static void uart_isr(const struct device *dev, void *user_data)
// {
// 	uart_irq_update(dev);

// 	/* RX: 逐字节读出放入 ring_buf */
// 	while (uart_irq_rx_ready(dev)) {
// 		uint8_t ch;
// 		if (uart_fifo_read(dev, &ch, 1) == 1) {
// 			ring_buf_put(&rx_ring, &ch, 1);
// 		}
// 	}

// 	if (uart_irq_tx_ready(dev)) {
//         send_next_data();
//     }
// }
