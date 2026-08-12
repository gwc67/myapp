#ifndef __UART_IT_H
#define __UART_IT_H

#include "uart_base.h"
#include "zephyr/device.h"
#include "zephyr/sys/ring_buffer.h"
#include <stdbool.h>
#include <stdint.h>



struct uart_it_t{
    struct uart_base_t base;
    const struct device* uart_device_pst;
    struct ring_buf rx_ring_st;                 //接受缓冲
    struct ring_buf tx_ring_st;                 //发送缓冲
    uint8_t* rx_hw_puc;
    
    bool tx_busy_b;
};



#endif
