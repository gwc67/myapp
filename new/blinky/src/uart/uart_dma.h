#ifndef __UART_DMA_H
#define __UART_DMA_H

#include "uart_base.h"
#include "zephyr/device.h"
// #include "zephyr/sys/ring_buffer.h"
#include "my_ring/my_ring.h"
#include <stdbool.h>
#include <stdint.h>

#define RX_HW_BUF_NUM 2

struct uart_dma_cfg_t {

    const struct device* uart_device_pst;
    uint8_t* tx_hw_puc;
    uint32_t tx_hw_len_ul;

    uint8_t *rx_hw_bufs_puc[RX_HW_BUF_NUM];   
    uint32_t rx_hw_lens_puc[RX_HW_BUF_NUM];
};


struct uart_dma_t{
    struct uart_base_t base;
    const struct uart_dma_cfg_t* cfg_pst;
    struct ring_buf_base_t* tx_ring_pst;

    struct ring_buf_base_t* rx_ring_pst;

    uint8_t free_rx_idx_uc;
    
    volatile bool tx_busy_b;
};

int uart_dma_init_rt(struct uart_dma_t* me, const struct uart_dma_cfg_t* cfg_pst, struct ring_buf_base_t* rx_ring_pst,struct ring_buf_base_t* tx_ring_pst);


#endif
