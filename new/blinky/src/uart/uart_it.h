#ifndef __UART_IT_H
#define __UART_IT_H

#include "uart_base.h"
#include "zephyr/device.h"
// #include "zephyr/sys/ring_buffer.h"
#include "my_ring/my_ring.h"
#include <stdbool.h>
#include <stdint.h>

struct uart_it_cfg_t {

    const struct device* uart_device_pst;
    uint8_t* tx_hw_puc;
    uint32_t tx_hw_len_ul;
    uint8_t* rx_hw_puc;
    uint32_t rx_hw_len_ul;
};


struct uart_it_t{
    struct uart_base_t base;
    const struct uart_it_cfg_t* cfg_pst;
    struct ring_buf_base_t* tx_ring_pst;
    struct ring_buf_base_t* rx_ring_pst;
    
    volatile bool tx_busy_b;
};



#endif
