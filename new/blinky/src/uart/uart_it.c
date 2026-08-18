#include "uart_it.h"
#include "my_ring.h"
#include "uart_base.h"
#include "zephyr/drivers/uart.h"
#include "zephyr/sys/util.h"
#include "zephyr/syscalls/uart.h"
#include "zephyr/toolchain.h"
#include <stdbool.h>
#include <stdint.h>
#include <sys/_intsup.h>
#include <sys/errno.h>


static void s_uart_isr(const struct device *dev, void *user_data)
{
     struct uart_it_t* me = (struct uart_it_t* )user_data;

     uart_irq_update(dev);

    if(uart_irq_tx_ready(dev))
    {
        uint8_t byte;
        if (my_ring_buf_get(me->tx_ring_pst, &byte, 1) == 1) {
            uart_fifo_fill(dev, &byte, 1);
        }
        else {
            uart_irq_tx_disable(dev);
            me->tx_busy_b = false;
        }
    }

    if (uart_irq_rx_ready(dev)) {
        uint8_t byte_uc = 0;
        while ((uart_fifo_read(dev, &byte_uc, 1) > 0)) {
            my_ring_buf_put(me->rx_ring_pst, &byte_uc, 1);
        }
    }
    
     
}

static int s_uart_transmit_it(uart_base_t* base,uint8_t* data_puc ,uint32_t len_ul)
{
    struct uart_it_t* me = CONTAINER_OF(base, struct uart_it_t, base);
    
    if (!me->uart_it_cfg_pst->uart_device_pst || !data_puc || len_ul == 0) {
        return -EINVAL;
    }

   if (my_ring_buf_put(me->tx_ring_pst, data_puc, len_ul) < len_ul) {
        return -EAGAIN;;
   }

   if (!me->tx_busy_b) {
    me->tx_busy_b = true;
    uart_irq_tx_enable(me->uart_it_cfg_pst->uart_device_pst);
   }
    return 0;
    
}

static int s_uart_rx_enable_it(struct uart_base_t* base,uint32_t timeout_ul)
{
    struct uart_it_t* me = CONTAINER_OF(base, struct uart_it_t, base);

    ARG_UNUSED(timeout_ul);

    if (!me->uart_it_cfg_pst->uart_device_pst) {
          return -ENODEV;
    }

    uart_irq_rx_enable(me->uart_it_cfg_pst->uart_device_pst);

    return 0;
}


const uart_ops_t uart_it_ops_st = 
{
    .uart_transmit = s_uart_transmit_it,
    .uart_rx_enable = s_uart_rx_enable_it,
};

int uart_it_init_rt(struct uart_it_t* me, const struct uart_it_cfg_t* cfg_pst, struct ring_buf_base_t* rx_ring_pst,struct ring_buf_base_t* tx_ring_pst)
{
    me->uart_it_cfg_pst = cfg_pst;
    me->rx_ring_pst = rx_ring_pst;
    me->tx_ring_pst = tx_ring_pst;
    me->tx_busy_b = false;
    me->base.ops = &uart_it_ops_st;
    //将这个设备绑定的父类的指针同时传入到那个回调函数中
    return uart_irq_callback_user_data_set(cfg_pst->uart_device_pst, s_uart_isr,me);
    
}

