#include "encode.h"
#include "zephyr/device.h"
#include "zephyr/drivers/sensor.h"
#include "zephyr/init.h"
#include "zephyr/kernel.h"
#include "zephyr/logging/log.h"
#include <complex.h>
#include <stdint.h>
#include <sys/errno.h>

LOG_MODULE_REGISTER(encoder,LOG_LEVEL_INF);

struct encoder_device_t
{
    const struct device *encoder_device_pst;
    uint32_t last_tick_count_ul;
    int32_t last_count_l;
    int32_t delta_l;
    struct encoder_data_t data_st;
};

static struct encoder_device_t s_encoder_device_pst[ENCODE_ID_NUM_em];

#define  ENCODE_CPR  44


static int encoder_init(void)
{
    s_encoder_device_pst[ENCODE_ID_A_em].encoder_device_pst = DEVICE_DT_GET(DT_ALIAS(encodera));
    s_encoder_device_pst[ENCODE_ID_B_em].encoder_device_pst = DEVICE_DT_GET(DT_ALIAS(encoderb));

    for (int i = 0; i < ENCODE_ID_NUM_em; i++) {
        if (!(device_is_ready(s_encoder_device_pst[i].encoder_device_pst))) {
            LOG_ERR("Encode %d device not ready",i);
            return -ENODEV;
        }

        s_encoder_device_pst[i].data_st.position_l = 0;
        s_encoder_device_pst[i].data_st.rpm_f = 0.0f;
        s_encoder_device_pst[i].delta_l = 0;
        s_encoder_device_pst[i].last_count_l = 0;
        
        LOG_INF("Encode %d initialized",i);
    }

    return 0;    
}

SYS_INIT(encoder_init, APPLICATION, 10);

int encoder_read(enum encoder_id_e id_em)
{
    if (id_em >= ENCODE_ID_NUM_em) {
        return -EINVAL;
    }

    
    uint32_t current_tick_ul;
    uint32_t last_tick_ul;
    struct sensor_value val_st;
    int ret;

    ret = sensor_sample_fetch(s_encoder_device_pst[id_em].encoder_device_pst);
    if (ret != 0) {
        LOG_ERR("Encoder %d sample fetch failed: %d",id_em,ret);
        return ret;
    }



    ret = sensor_channel_get(s_encoder_device_pst[id_em].encoder_device_pst,SENSOR_CHAN_ROTATION ,&val_st);

    if (ret != 0) {
        LOG_ERR("Encoder %d channel get failed : %d",id_em,ret);
        return ret;
    }


    current_tick_ul = k_uptime_ticks();
    last_tick_ul = s_encoder_device_pst[id_em].last_tick_count_ul;

    /* val_st.val = 整数度，val.val2 = 小数度（百万分之一） */
    int32_t degrees_l = val_st.val1;  
    int32_t current_count_l = (degrees_l * ENCODE_CPR) / 360;

    s_encoder_device_pst[id_em].delta_l = current_count_l - s_encoder_device_pst[id_em].last_count_l;

    s_encoder_device_pst[id_em].data_st.position_l += s_encoder_device_pst[id_em].delta_l;
    s_encoder_device_pst[id_em].last_count_l  = current_count_l;

    s_encoder_device_pst[id_em].data_st.rpm_f = ((float)s_encoder_device_pst[id_em].delta_l / ENCODE_CPR) * (60000.0f/(current_tick_ul - last_tick_ul));
    s_encoder_device_pst[id_em].last_tick_count_ul = current_tick_ul;
    return 0;
}

void encoder_update_all(void)
{
    for (int i = 0; i < ENCODE_ID_NUM_em; i++) {
        encoder_read(i);
    }
}


int encoder_get_data(enum encoder_id_e id_em, struct encoder_data_t* out)
{
     if (id_em >= ENCODE_ID_NUM_em) {
        return -ENODEV;
    }
    *out = s_encoder_device_pst[id_em].data_st;
    return 0;
}

