#ifndef  __UART_IT_H
#define  __UART_IT_H


#include "my_ring.h"
#include "uart_base.h"
#include "zephyr/device.h"

struct uart_it_cfg_t
{
    const struct device* uart_device_pst;
};

struct uart_it_t {
    struct uart_base_t base;
    const struct uart_it_cfg_t* uart_it_cfg_pst; 
    struct ring_buf_base_t* rx_ring_pst;
    struct ring_buf_base_t* tx_ring_pst;

    volatile bool tx_busy_b;            //标记是否有待发送的数据/TX中断已经使能？

    //TX中断已经开启，且ring中有数据，将作为tx_busy_b
};

int uart_it_init_rt(struct uart_it_t* me, const struct uart_it_cfg_t* cfg_pst, struct ring_buf_base_t* rx_ring_pst,struct ring_buf_base_t* tx_ring_pst);



#endif


