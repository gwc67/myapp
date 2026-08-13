#include "ano_device_com.h"
#include "ano.h"
#include "ano_base.h"
#include "uart_base.h"
#include "zephyr/init.h"
#include <stdint.h>
#include <string.h>
#include "uarts.h"


static int s_ano_device_com_init(void)
{
    ano_set_send_id(g_com_ano_pst, 0x00, 0);
    ano_set_send_id(g_com_ano_pst, 0x01, 20);
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
    // else if (*(data_puc + 2) == 0xE1)
    // {
    //     uint16_t par_id = *(pucData + 4) + *(pucData + 5) * 256;
    //     ano_par_back_v(pstAnobase_Com, 0xff, par_id, 0);
    // }
    // else if (*(pucData + 2) == 0xE2)
    // {
    //     _par_st snap = {0};
    //     snap.par_id = *(pucData + 4) + *(pucData+5)*256;
    //     snap.par_val = *((int32_t*)(pucData + 6));
    //     ano_ck_back_v(pstAnobase_Com, 0xff, *(pucData + 2), check_sum1, check_sum2);
    //     temp_st.x_s = snap.par_val;
           
    // }

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

        default:
            break;
    }
}


void com_send_buffer(uint8_t *data_puc,uint8_t len_uc)
{
    uart_transmit(g_uart3_pst, data_puc, len_uc);
}

void com_check_to_send(void)
{
    ano_ck_back_check(g_com_ano_pst);
    ano_check_to_send(g_com_ano_pst, 0x00);
    ano_check_to_send(g_com_ano_pst, 0xe0);
    ano_check_to_send(g_com_ano_pst, 0x01);
}
