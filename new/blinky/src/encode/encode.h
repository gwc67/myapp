#ifndef __ENCODE_H
#define __ENCODE_H

#include <stdint.h>
enum encoder_id_e
{
    ENCODE_ID_A_em = 0,
    ENCODE_ID_B_em,
    ENCODE_ID_NUM_em,
};

struct encoder_data_t
{
    int32_t position_l;  //当前位置，累计量
    float   rpm_f;       //转速     
};

int encoder_read(enum encoder_id_e id_em);
void encoder_update_all(void);
int encoder_get_data(enum encoder_id_e id_em, struct encoder_data_t* out);


#endif
