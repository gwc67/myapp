#include "ano_base.h"
#include "ano_true.h"
#include "zephyr/init.h"
#include <stdint.h>
#include <string.h>
#include "ano_device/ano_device_com.h"

static struct ano_device_t s_com_st;
static uint8_t s_com_rx_buffer_pst[FRAME_MAX_LENGTH];
static struct ano_frame_t s_com_frame_st;
static const private_t s_com_private_st = {
    .ano_add_send_data = com_add_send_data,
    .ano_send_buffer = com_send_buffer,
    .ano_receive_anl = com_receive_anl,
};

struct ano_base_t* g_com_ano_pst;

int ano_board_init(void)
{
    int result = 0;

    static const struct ano_cfg_t s_com_cfg_st = {
        .private_pst = &s_com_private_st,
        .rx_buffer_puc = s_com_rx_buffer_pst,
        .ring_buf_base_ppst = &g_ut3_rx_ring_pst,
    };

    result = ano_device_init_noraml(&s_com_st, &s_com_frame_st, &s_com_cfg_st);
    if (result != 0) {
        return -1;
    }
    

    g_com_ano_pst = &s_com_st.base;

    return result;

}

//在uart_board_init 和 ring_buf 之后进行初始化
SYS_INIT(ano_board_init, APPLICATION, 13);