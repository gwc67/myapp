#include "uart_base.h"
#include "zephyr/device.h"
#include "zephyr/drivers/uart.h"


void uart_async_isr(const struct device* dev,struct uart_event* evt,void* user_data)
{
    struct uart_base_t *me = (struct uart_base_t* )user_data;
    
    switch (evt->type) {
        case UART_TX_DONE:
            if (!my_ring_buf_is_empty(me)) {
            
            }
    }
}
