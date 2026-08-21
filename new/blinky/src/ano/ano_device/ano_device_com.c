#include "ano_device_com.h"
#include "ano.h"
#include "ano_base.h"
#include "encode.h"
#include "euler.h"
#include "uart_base.h"
#include "zephyr/drivers/gpio.h"
#include "zephyr/init.h"
#include <stdint.h>
#include <string.h>
#include "uarts.h"

#define SIMULINK_DATA_TX 0x02

#define SIMULINK_PID_RX  0x03

static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

static int s_ano_device_com_init(void)
{
    ano_set_send_id(g_com_ano_pst, 0x00, 0);
    ano_set_send_id(g_com_ano_pst, 0x01, 20);
    // ano_set_send_id(g_com_ano_pst, SIMULINK_DATA_TX, 10);
    return 0;
}

SYS_INIT(s_ano_device_com_init, APPLICATION, 14);


void com_receive_anl(uint8_t* data_puc,uint8_t len_uc)
{
    uint8_t check_sum1 = 0, check_sum2 = 0;
    if (*(data_puc + 3) != (len_uc - 6))
    {
        return;
    }
    for (uint8_t i = 0; i < len_uc - 2; i++)
    {
        check_sum1 += *(data_puc + i);
        check_sum2 += check_sum1;
    }

    if ((check_sum1 != *(data_puc + len_uc - 2)) || (check_sum2 != *(data_puc + len_uc - 1)))
    {
        return;
    }
    
    if(*(data_puc + 2) == 0x00)
    {
        struct check_back_t snap = {0};
        if (snap.id_uc == *(data_puc + 4) && snap.sc_uc == *(data_puc + 5) && snap.ac_uc == *(data_puc + 6)) {
            ano_clear_wait(g_com_ano_pst);
        }
    }
    else if (*(data_puc + 2) == 0xe0)
    {

        struct check_back_t check_back_st = {0};
        check_back_st.id_uc = *(data_puc + 2);
        check_back_st.sc_uc = check_sum1;
        check_back_st.ac_uc = check_sum2;
        ano_set_check_back(g_com_ano_pst,&check_back_st);
    }
    else if(*(data_puc + 2) == SIMULINK_PID_RX)
    {
        // uart_transmit(g_uart2_pst, "pid_adjust\r\n",13 );
        gpio_pin_toggle_dt(&led0);

    }

}


void com_add_send_data(uint8_t frame_num_uc,uint8_t *cnt_puc,uint8_t* data_puc)
{
    switch (frame_num_uc) {
        case 0x00:
        {
            struct check_back_t snap = {0};
            ano_get_check_back(g_com_ano_pst, &snap);
            memcpy(data_puc + *cnt_puc, &snap, sizeof(snap));
            *cnt_puc += sizeof(snap);
        }
        break;
        case 0x01:
        {
            data_puc[(*cnt_puc)++] = 0x01;
        }
        break;
        case SIMULINK_DATA_TX:
        {
            // struct euler_t euler_st;
            // euler_copy(&euler_st);
            // memcpy(data_puc + *cnt_puc,&euler_st.pitch_db,sizeof(euler_st.pitch_db));
            // *cnt_puc += sizeof(euler_st.pitch_db);

            // struct encoder_data_t encoder_data_st;
            // encoder_get_data(g_encoder_a_pst, &encoder_data_st);

            // memcpy(data_puc + *cnt_puc,&encoder_data_st.rpm_l,sizeof(encoder_data_st.rpm_l));
            // *cnt_puc += sizeof(encoder_data_st.rpm_l);

            // encoder_get_data(g_encoder_b_pst, &encoder_data_st);
            // memcpy(data_puc + *cnt_puc,&encoder_data_st.rpm_l,sizeof(encoder_data_st.rpm_l));
            // *cnt_puc += sizeof(encoder_data_st.rpm_l);
            
            // data_puc[(*cnt_puc)++] = '\r';
            // data_puc[(*cnt_puc)++] = '\n';
        }
        break;
        default:
            break;
    }
}


void com_send_buffer(uint8_t *data_puc,uint8_t len_uc)
{
    uart_transmit(g_uart2_pst, data_puc, len_uc);
}

void com_check_to_send(void)
{
    ano_ck_back_check(g_com_ano_pst);
    ano_check_to_send(g_com_ano_pst, 0x00);
    ano_check_to_send(g_com_ano_pst, 0xe0);
    // ano_check_to_send(g_com_ano_pst, SIMULINK_DATA_TX);
    // ano_check_to_send(g_com_ano_pst, 0x01);
}
