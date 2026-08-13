#include "uart_it.h"
#include "my_ring/my_ring.h"
#include "uart_base.h"
#include "zephyr/device.h"
#include "zephyr/drivers/uart.h"
#include "zephyr/sys/time_units.h"
#include "zephyr/sys/util.h"
#include "zephyr/syscalls/uart.h"
#include <stdbool.h>
#include <stdint.h>
#include <sys/_intsup.h>
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
    if (!me->cfg_pst->uart_device_pst || !data_puc || len_ul == 0) {
        return  -EINVAL;
    }
    
    if (!me->tx_busy_b) {
        me->tx_busy_b = true;
        int ret = uart_tx(me->cfg_pst->uart_device_pst, data_puc, len_ul, SYS_FOREVER_US);

        if (ret != 0) {
            //启动失败
            me->tx_busy_b = false;
            return -EIO;
        }
    }
    else {
        uint32_t written_ul = my_ring_buf_put(me->tx_ring_pst, data_puc, len_ul);
        if (written_ul < len_ul) {
            return -EAGAIN;             //部分写入
        }
    }
    return 0;
}

static int s_uart_rx_enable_it(uart_base_t* base,uint32_t timeout_ul)
{
    struct uart_it_t* me = CONTAINER_OF(base, struct uart_it_t, base);

    if (!me->cfg_pst->uart_device_pst) {
        return -ENODEV;
    }
    int ret = uart_rx_enable(me->cfg_pst->uart_device_pst, me->cfg_pst->rx_hw_bufs_puc[0], me->cfg_pst->rx_hw_lens_puc[0], timeout_ul);

    if (ret != 0) {
        return ret;
    }
    return 0;
}

//中断给的是设备号，需要使用设备号反推ring_buf的对象
//uart_device_pst 所指向的地址，并不在uart_it 设备里面，可以使用中断注册的user_data， 那么问题来了在哪里注册对应的uart_async_isr呢？
//需要在board_init中进行注册

static void uart_async_isr(const struct device* dev,struct uart_event* evt,void* user_data)
{
    struct uart_it_t *me = (struct uart_it_t* )user_data;
    
    switch (evt->type) {
        case UART_TX_DONE:
        //缓冲发送完成，
            if (!my_ring_buf_is_empty(me->tx_ring_pst)) {
            
                uint32_t len_ul = my_ring_buf_get(me->tx_ring_pst, me->cfg_pst->tx_hw_puc, me->cfg_pst->tx_hw_len_ul);
                int ret = uart_tx(dev,me->cfg_pst->tx_hw_puc,len_ul,SYS_FOREVER_US);
                
                if (ret != 0) {
                    me->tx_busy_b = false; //启动失败，标记空闲
                }
            }
            else 
            {
                me->tx_busy_b = false;      //无后续数据，标记空闲
            }
            break;
        case UART_TX_ABORTED:
            me->tx_busy_b = false;
            break;
        case UART_RX_RDY:
            my_ring_buf_put(me->rx_ring_pst, &evt->data.rx.buf[evt->data.rx.offset], evt->data.rx.len);
            break;
        case UART_RX_BUF_REQUEST:
        if (me->cfg_pst->rx_hw_bufs_puc[1]) {
            me->free_rx_idx_uc ^= 1U; 
        }
            uart_rx_buf_rsp(dev, me->cfg_pst->rx_hw_bufs_puc[me->free_rx_idx_uc],me->cfg_pst->rx_hw_lens_puc[me->free_rx_idx_uc]);           //重新以当前rx_hw_buf为起点开启吗？也就是类似之前的重新开启it吗？
            break;
        case UART_RX_BUF_RELEASED:                                                   //IT模式下的释放不需要干任何事情
            break;
        default:
            break;

    }
}

const uart_ops_t uart_it_ops_st = {
    .uart_transmit = s_uart_transmit_it,
    .uart_rx_enable = s_uart_rx_enable_it,
};




//同时需要it发送和接受的初始化
int uart_it_init_rt(struct uart_it_t* me, const struct uart_it_cfg_t* cfg_pst, struct ring_buf_base_t* rx_ring_pst,struct ring_buf_base_t* tx_ring_pst)
{


    me->cfg_pst = cfg_pst;
    me->rx_ring_pst = rx_ring_pst;
    me->tx_ring_pst = tx_ring_pst;
    me->tx_busy_b = false;
    me->free_rx_idx_uc = 0;
    me->base.ops = &uart_it_ops_st;
    int ret = uart_callback_set(me->cfg_pst->uart_device_pst, uart_async_isr, me);
    if (ret != 0) {
        return ret;
    }
    return 0;
    
}

