#ifndef __ANO_DEVICE_COM_H
#define __ANO_DEVICE_COM_H


#include <stdint.h>

void com_send_buffer(uint8_t *data_puc,uint8_t len_uc);
void com_add_send_data(uint8_t frame_num_uc,uint8_t *cnt_puc,uint8_t* data_puc);
void com_receive_anl(uint8_t* data_puc,uint8_t len_uc);
void com_check_to_send(void);



#endif
