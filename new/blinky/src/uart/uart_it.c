#include "uart_it.h"
#include "uart_base.h"
#include "zephyr/sys/ring_buffer.h"
#include "zephyr/sys/time_units.h"
#include "zephyr/sys/util.h"
#include "zephyr/syscalls/uart.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <sys/errno.h>



/**
 * @brief 将数据写入 TX ring_buf，若设备空闲则启动首次传输
 * @return 0: 全部写入成功
 *         -EAGAIN: 部分写入（ring_buf 空间不足）
 *         -ENODEV: 无效设备
 */

//会有一个函数进行给它填充数据的
static int s_uart_transmit_it(uart_base_t* base,uint8_t* data_puc ,uint32_t len_ul)
{
    struct uart_it_t* me = CONTAINER_OF(base, struct uart_it_t, base);
    if (!me->uart_device_pst || !data_puc || len_ul == 0) {
        return  -EINVAL;
    }
    
    if (!me->tx_busy_b) {
        me->tx_busy_b = true;
        int ret = uart_tx(me->uart_device_pst, data_puc, len_ul, SYS_FOREVER_US);

        if (ret != 0) {
            //启动失败
            me->tx_busy_b = false;
            return -EIO;
        }
    }
    else {
        uint32_t written_ul = ring_buf_put(&me->tx_ring_st, data_puc, len_ul);
        if (written_ul < len_ul) {
            return -EAGAIN;             //部分写入
        }
    }
    return 0;
}

static int s_uart_rx_enable_it(uart_base_t* base,uint32_t timeout_ul)
{
    struct uart_it_t* me = CONTAINER_OF(base, struct uart_it_t, base);

    if (!me->uart_device_pst) {
        return -ENODEV;
    }
    int ret = uart_rx_enable(me->uart_device_pst, me->rx_hw_puc, sizeof(me->rx_hw_puc), timeout_ul);

    if (ret != 0) {
        return ret;
    }
    return 0;
}


/// ENOMEM 空的 期望拿len_ul 长度的数据
static int s_uart_get_rx_ring(uart_base_t* base,uint8_t* data_puc,enum )
{
    struct uart_it_t* me = CONTAINER_OF(base, struct uart_it_t, base);
    
    if (ring_buf_is_empty(&me->rx_ring_st)) {
        return -ENOMEM;
    }

    //默认期望当前全局数组的最大容量
    uint32_t actual_ul = ring_buf_get(&me->rx_ring_st, data_puc, sizeof(me->rx_hw_puc));
    return actual_ul;
    
}




