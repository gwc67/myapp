// /*
//  * UART 传输层 — 实现
//  *
//  * IT 和 DMA 两种后端共存于同一文件.
//  * 每个 uart_xfer 实例在 init 时选择模式, 互不干扰.
//  *
//  * IT  (无 CONFIG_UART_ASYNC_API 也能工作):
//  *   TX: ring_buf_put → uart_irq_tx_enable → ISR uart_fifo_fill
//  *   RX: ISR uart_fifo_read → rx_cb
//  *
//  * DMA (需要 CONFIG_UART_ASYNC_API + DTS dmas):
//  *   TX: k_mem_slab_alloc → memcpy → uart_tx → UART_TX_DONE → slab_free
//  *   RX: uart_rx_enable → UART_RX_RDY → rx_cb → uart_rx_buf_rsp
//  */

// #include "uart_xfer.h"

// #include <zephyr/drivers/uart.h>
// #include <zephyr/kernel.h>
// #include <zephyr/sys/ring_buffer.h>
// #include <string.h>

// /* 当前活动的实例 (ISR 中访问), 单实例情况下直接指向 g_ano_xfer */
// // static struct uart_xfer *s_active_xfer_pst;

// /* ================================================================
//  * IT 后端
//  * ================================================================ */

// static void s_it_isr(const struct device *dev_pst, void *user_data)
// {
// 	struct uart_xfer *xf_pst = user_data;				
// 	uint8_t *tx_ptr;
// 	uint8_t rx_byte;

// 	uart_irq_update(dev_pst);

// 	/* TX: claim ring_buf → 填硬件 DR 寄存器 */
// 	if (uart_irq_tx_ready(dev_pst)) {
// 		uint32_t avail = ring_buf_get_claim(&xf_pst->it_tx_ring,
// 						    &tx_ptr, 16);
// 		if (avail > 0) {
// 			uint32_t sent = uart_fifo_fill(dev_pst, tx_ptr, avail);
// 			ring_buf_get_finish(&xf_pst->it_tx_ring, sent);
// 		}
// 		if (ring_buf_is_empty(&xf_pst->it_tx_ring)) {
// 			uart_irq_tx_disable(dev_pst);
// 		}
// 	}

// 	/* RX: 逐字节读 → 回调 */
// 	if (xf_pst->rx_cb_p) {
// 		while (uart_irq_rx_ready(dev_pst)
// 		       && uart_fifo_read(dev_pst, &rx_byte, 1) == 1) {
// 			xf_pst->rx_cb_p(&rx_byte, 1, xf_pst->rx_user_data);
// 		}
// 	}
// }

// static int s_it_init(struct uart_xfer *xf)
// {
// 	if (!device_is_ready(xf->dev)) {
// 		return -ENODEV;
// 	}

// 	ring_buf_init(&xf->it_tx_ring, UART_XFER_IT_TX_SIZE, xf->it_tx_buf);

// 	uart_irq_callback_user_data_set(xf->dev, s_it_isr, xf);					//这步将
// 	uart_irq_rx_enable(xf->dev);

// 	return 0;
// }

// static void s_it_send(struct uart_xfer *xf,
// 		      const uint8_t *data, size_t len)
// {
// 	ring_buf_put(&xf->it_tx_ring, data, len); /* 唯一一次拷贝 */
// 	uart_irq_tx_enable(xf->dev);
// }

// /* ================================================================
//  * DMA / Async 后端 (需要 CONFIG_UART_ASYNC_API)
//  * ================================================================ */

// #ifdef CONFIG_UART_ASYNC_API

// static void s_dma_cb(const struct device *dev,
// 		     struct uart_event *evt, void *user_data)
// {
// 	struct uart_xfer *xf = user_data;

// 	switch (evt->type) {

// 	case UART_TX_DONE:
// 		k_mem_slab_free(&xf->dma_tx_slab, (void **)&evt->data.tx.buf);
// 		break;

// 	case UART_TX_ABORTED:
// 		k_mem_slab_free(&xf->dma_tx_slab, (void **)&evt->data.tx.buf);
// 		break;

// 	case UART_RX_RDY:
// 		if (xf->rx_cb) {
// 			xf->rx_cb(xf->dma_rx_buf + evt->data.rx.offset,
// 				  evt->data.rx.len, xf->rx_user_data);
// 		}
// 		uart_rx_buf_rsp(dev, xf->dma_rx_buf, DMA_RX_BUF_SIZE);
// 		break;

// 	case UART_RX_DISABLED:
// 		uart_rx_enable(dev, xf->dma_rx_buf, DMA_RX_BUF_SIZE,
// 			       SYS_FOREVER_US);
// 		break;

// 	default:
// 		break;
// 	}
// }

// static int s_dma_init(struct uart_xfer *xf)
// {
// 	if (!device_is_ready(xf->dev)) {
// 		return -ENODEV;
// 	}

// 	k_mem_slab_init(&xf->dma_tx_slab, xf->dma_slab_buf,
// 			DMA_SLAB_BLOCK, DMA_SLAB_COUNT);

// 	uart_callback_set(xf->dev, s_dma_cb, xf);
// 	uart_rx_enable(xf->dev, xf->dma_rx_buf, DMA_RX_BUF_SIZE,
// 		       SYS_FOREVER_US);

// 	return 0;
// }

// static void s_dma_send(struct uart_xfer *xf,
// 		       const uint8_t *data, size_t len)
// {
// 	uint8_t *buf;

// 	if (k_mem_slab_alloc(&xf->dma_tx_slab, (void **)&buf, K_NO_WAIT) != 0) {
// 		return;  /* 池耗尽, 丢弃 (等同 xQueueSend 0 超时) */
// 	}
// 	memcpy(buf, data, len);                       /* 唯一一次拷贝 */
// 	uart_tx(xf->dev, buf, len, SYS_FOREVER_US);
// }

// #else /* !CONFIG_UART_ASYNC_API */

// static int s_dma_init(struct uart_xfer *xf)
// {
// 	(void)xf;
// 	return -ENOTSUP;  /* 未启用 CONFIG_UART_ASYNC_API */
// }

// static void s_dma_send(struct uart_xfer *xf,
// 		       const uint8_t *data, size_t len)
// {
// 	(void)xf; (void)data; (void)len;
// }

// #endif /* CONFIG_UART_ASYNC_API */

// /* ================================================================
//  * 公共 API
//  * ================================================================ */

// int uart_xfer_init(struct uart_xfer *xfer, const struct device *dev,
// 		   enum uart_xfer_mode_e mode_em,
// 		   uart_xfer_rx_cb_t rx_cb, void *user_data)
// {
// 	if (!xfer || !dev) {
// 		return -EINVAL;
// 	}

// 	xfer->dev   = dev;
// 	xfer->mode_em  = mode_em;
// 	xfer->rx_cb_p = rx_cb;
// 	xfer->rx_user_data = user_data;

// 	switch (mode_em) {
// 	case UART_XFER_MODE_IT:
// 		return s_it_init(xfer);
// 	case UART_XFER_MODE_DMA:
// 		return s_dma_init(xfer);
// 	default:
// 		return -EINVAL;
// 	}
// }

// //之后可以通过更改mode来实现对应的不同的发送，那本质不还是绑定操作表函数吗？
// //AI设计的架构还是有问题
// void uart_xfer_send(struct uart_xfer *xfer, const uint8_t *data, size_t len)
// {
// 	if (!xfer || !data || len == 0) {
// 		return;
// 	}

// 	switch (xfer->mode_em) {
// 	case UART_XFER_MODE_IT:
// 		s_it_send(xfer, data, len);
// 		break;
// 	case UART_XFER_MODE_DMA:
// 		s_dma_send(xfer, data, len);
// 		break;
// 	}
// }
