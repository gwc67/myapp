
#ifndef     __UART_BASE_H
#define     __UART_BASE_H

#include <stdint.h>

typedef struct uart_base_t uart_base_t ;

typedef struct uart_ops_t {
   int (*uart_transmit)(uart_base_t* me,uint8_t data_puc ,uint32_t len_ul);
};


#endif
