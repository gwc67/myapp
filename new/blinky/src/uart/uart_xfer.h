// /*
//  * UART 传输层 — 每个 UART 实例独立选择 IT 或 DMA 后端
//  *
//  * 设计原则:
//  *   - 不使用全局 #ifdef CONFIG_UART_ASYNC_API 互斥
//  *   - IT 和 DMA 两种后端共存于同一固件
//  *   - 每个实例在 uart_xfer_init() 时显式指定模式
//  *
//  * 用法 (ANO 在 ano_board_init.c 中):
//  *   static struct uart_xfer g_ano_xfer;
//  *   uart_xfer_init(&g_ano_xfer, DEVICE_DT_GET(DT_NODELABEL(usart2)),
//  *                  UART_XFER_MODE_DMA, rx_callback, NULL);
//  *   uart_xfer_send(&g_ano_xfer, data, len);
//  */

// #ifndef __UART_XFER_H
// #define __UART_XFER_H

// #include <stddef.h>
// #include <stdint.h>
// #include <zephyr/device.h>
// #include <zephyr/kernel.h>
// #include <zephyr/sys/ring_buffer.h>

// /* 传输模式 — 每个实例独立选择 */
// enum uart_xfer_mode_e {
// 	UART_XFER_MODE_IT,    /* 中断驱动 (uart_irq_* + ring_buf)    */
// 	UART_XFER_MODE_DMA,   /* DMA/异步 (uart_tx + uart_rx_enable) */
// };

// /* RX 回调: 收到字节时调用 (中断/DMA 回调上下文, 不要阻塞) */
// typedef void (*uart_xfer_rx_cb_t)(const uint8_t *data, size_t len,
// 				  void *user_data);

// /* ── 传输实例 (用户声明为 static 变量) ── */
// #define UART_XFER_IT_TX_SIZE  512
// #define UART_XFER_DMA_BLOCK   64
// #define UART_XFER_DMA_COUNT   4
// #define UART_XFER_DMA_RX_SIZE 256

// struct uart_xfer {
// 	const struct device *dev;
// 	enum uart_xfer_mode_e mode_em;
// 	uart_xfer_rx_cb_t rx_cb_p;				//函数指针 这个应该是解析指针 整个设计哲学有问题，为什么，因为如果是在中断中解析
// 	void            *rx_user_data;

// 	/* IT 后端 */
// 	struct ring_buf  it_tx_ring;
// 	uint8_t          it_tx_buf[UART_XFER_IT_TX_SIZE];

// #ifdef CONFIG_UART_ASYNC_API
// 	/* DMA 后端 */
// 	struct k_mem_slab dma_tx_slab;
// 	uint8_t           dma_slab_buf[UART_XFER_DMA_COUNT * UART_XFER_DMA_BLOCK] __aligned(4);
// 	uint8_t           dma_rx_buf[UART_XFER_DMA_RX_SIZE];
// #endif
// };

// /* ── API ── */

// /* 初始化一个 UART 传输实例.
//  * xfer:   用户分配的结构体 (static 或全局)
//  * dev:    UART 设备 (DEVICE_DT_GET)
//  * mode:   IT 或 DMA, 每个实例独立选择
//  * rx_cb:  收到数据时的回调 (NULL = 不启用 RX)
//  * user_data: 回调透传参数
//  * 返回 0 成功, 负值失败 */
// int uart_xfer_init(struct uart_xfer *xfer, const struct device *dev,
// 		   enum uart_xfer_mode_e mode_em,
// 		   uart_xfer_rx_cb_t rx_cb, void *user_data);

// /* 非阻塞发送. IT 模式: ring_buf_put → 唤醒 TX ISR.
//  * DMA 模式: k_mem_slab_alloc → memcpy → uart_tx */
// void uart_xfer_send(struct uart_xfer *xfer_pst, const uint8_t *data, size_t len);

// #endif /* __UART_XFER_H */
