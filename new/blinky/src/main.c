/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/display/cfb.h>
#include <zephyr/sys/printk.h>

#include "menu/menu.h"
#include "zephyr/drivers/uart.h"
#include "zephyr/kernel/thread.h"
#include "zephyr/kernel/thread_stack.h"
#include "zephyr/posix/sys/stat.h"
#include "zephyr/sys/ring_buffer.h"
#include "zephyr/sys/time_units.h"
#include "zephyr/syscalls/uart.h"
#include "zephyr/toolchain.h"

#define UART_NODE DT_NODELABEL(usart2)

static const struct device *uart_dev_pst = DEVICE_DT_GET(UART_NODE);

/* 环形缓冲区 */
#define RX_BUF_SIZE 32
#define TX_BUF_SIZE 32

/* RX 双缓冲轮转 */
static uint8_t rx_buf_a[RX_BUF_SIZE];
static uint8_t rx_buf_b[RX_BUF_SIZE];

/* ISR -> main 传递 */
static uint8_t rx_ring_buf_puc[RX_BUF_SIZE];
static struct ring_buf rx_ring;
static uint8_t tx_ring_buf_puc[TX_BUF_SIZE];  //2的次方
static struct ring_buf tx_ring;

struct tx_msg_t {
	uint8_t* data_puc;
	uint32_t len_ul;
};

K_MSGQ_DEFINE(tx_msgq, sizeof(struct tx_msg_t), 16,4);

static K_SEM_DEFINE(tx_done_sem, 0, 1);

static volatile uint8_t *free_buf = NULL; 


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

			free_buf = evt->data.rx_buf.buf;
		
			break;
		case UART_RX_BUF_REQUEST:

			if (free_buf != NULL) {
				int ret =  uart_rx_buf_rsp(uart_dev_pst, (uint8_t*)free_buf, RX_BUF_SIZE);
				if (ret == 0) {
					free_buf = NULL;
				}
			}
		
			break;

		case UART_RX_DISABLED:
			/* 缓冲区写满 / 出错时驱动会停掉 DMA 接收.
			 * 单缓冲方案必须在这里重新使能, 否则收一次 128 字节就永久停了.
			 * (该事件在线程/工作队列上下文发出, 这里可以直接调用)
			 */
			uart_rx_enable(uart_dev_pst, rx_buf_a, RX_BUF_SIZE, 1000);
			break;
		case UART_TX_DONE:
			k_sem_give(&tx_done_sem);
			break;

			//中断被中止
		case UART_TX_ABORTED:
			free_buf = NULL;
        // 重启逻辑...
        	uart_rx_enable(uart_dev_pst, rx_buf_a, RX_BUF_SIZE, 1000);
        // 注意：重启后，需要重新注册备用缓冲区
        	uart_rx_buf_rsp(uart_dev_pst, rx_buf_b, RX_BUF_SIZE);
			break;
		default:
			break;
	}
}

static void tx_thread_fn(void *p1,void*p2,void* p3)
{
	ARG_UNUSED(p1); ARG_UNUSED(p2); ARG_UNUSED(p3);

	struct tx_msg_t tx_msg_st = {0};

	while (1) {
		k_msgq_get(&tx_msgq, &tx_msg_st, K_FOREVER);

		int ret = uart_tx(uart_dev_pst, tx_msg_st.data_puc, tx_msg_st.len_ul, SYS_FOREVER_US);
		if (ret != 0) {
			continue;
		}

		k_sem_take(&tx_done_sem, K_FOREVER);
		
	}


}


int uart_tx_send(uint8_t *data_puc,uint32_t len_ul)
{
	struct tx_msg_t tx_msg_st = {	.data_puc = data_puc,.len_ul = len_ul};
	return  k_msgq_put(&tx_msgq, &tx_msg_st, K_NO_WAIT);
}


// static void uart_isr(const struct device *dev, void *user_data);
// static void send_next_data(void);

int main(void)
{
	ring_buf_init(&rx_ring, RX_BUF_SIZE, rx_ring_buf_puc);
	ring_buf_init(&tx_ring, TX_BUF_SIZE, tx_ring_buf_puc);

	//创建TX线程
	static K_THREAD_STACK_DEFINE(tx_stack,512);			//填栈大小
	static struct k_thread tx_thread;
	k_thread_create(&tx_thread, tx_stack, 512, tx_thread_fn, NULL, NULL, NULL, 5, 0, K_NO_WAIT);
	k_thread_name_set(&tx_thread, "uart_tx");


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

	uart_rx_buf_rsp(uart_dev_pst, rx_buf_b, RX_BUF_SIZE);
	
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
        uint8_t data_buf[100] = {0};

        while (1) {

		/* 从 RX ring_buf 取出字节 → 回显到 TX */
		// while (ring_buf_get(&rx_ring, &ch, 1) == 1) {
		// 	if (!tx_busy) {
		// 		tx_buf[0] = ch;
		// 		tx_busy = true;
		// 		uart_tx(uart_dev_pst,tx_buf,1,SYS_FOREVER_US);
		// 	}
		// }

		uint32_t actual = ring_buf_get(&rx_ring, data_buf, RX_BUF_SIZE);
		if (actual > 0) {
			uart_tx_send(data_buf,actual);
		}


		// if (!tx_busy) {



		// 	if (len > 0) {
		// 		tx_claimed_len = len ;
		// 		tx_busy = true;
		// 	}
		// }
		
		
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
