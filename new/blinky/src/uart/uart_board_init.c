#include "my_ring/my_ring.h"
#include "uart_base.h"
#include "uart_it.h"
#include "zephyr/device.h"
#include "zephyr/init.h"
#include <stdint.h>



#define UT2_RX_HW_LENGH 128
#define UT2_TX_HW_LENGH 64
static struct uart_it_t s_uart2_st;
static uint8_t s_ut2_rx_hw_a_puc[UT2_RX_HW_LENGH];
static uint8_t s_ut2_rx_hw_b_puc[UT2_RX_HW_LENGH];
static uint8_t s_ut2_tx_hw_puc[UT2_TX_HW_LENGH];

struct uart_base_t* g_uart2_pst;

int uart_board_init(void)
{
    static const struct uart_it_cfg_t uart2_it_cfg_st = {
        .uart_device_pst = DEVICE_DT_GET(DT_NODELABEL(usart2)),
        .rx_hw_bufs_puc[0] = s_ut2_rx_hw_a_puc,
        .rx_hw_lens_puc[0] = UT2_RX_HW_LENGH,
        .rx_hw_bufs_puc[1] = s_ut2_rx_hw_b_puc,
        .rx_hw_lens_puc[1] = UT2_RX_HW_LENGH,
        .tx_hw_puc = s_ut2_tx_hw_puc,
        .tx_hw_len_ul = UT2_TX_HW_LENGH,
    };
    int ret = uart_it_init_rt(&s_uart2_st, &uart2_it_cfg_st, g_ut2_rx_ring_pst,g_ut2_tx_ring_pst);

    if (ret) {
        return ret;
    }
    g_uart2_pst = &s_uart2_st.base;


    ret = uart_receive_enable(g_uart2_pst, 1000);  
    
    
    return 0;
    
}

SYS_INIT(uart_board_init, APPLICATION, 11);


