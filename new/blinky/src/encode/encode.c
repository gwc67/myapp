#include "encode.h"
#include "OLED_Menu.h"
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
    int32_t last_raw_l; 
    uint32_t last_tick_ul;
    struct encoder_data_t data_st;
};

static struct encoder_device_t s_encoder_device_pst[ENCODE_ID_NUM_em];

#define  ENCODE_CPR  44

#define TIM_ARR         65493   /* UINT16_MAX - (UINT16_MAX % 44) - 1 */

static int encoder_init(void)
{
    s_encoder_device_pst[ENCODE_ID_A_em].encoder_device_pst = DEVICE_DT_GET(DT_ALIAS(encodera));
    s_encoder_device_pst[ENCODE_ID_B_em].encoder_device_pst = DEVICE_DT_GET(DT_ALIAS(encoderb));

    for (int i = 0; i < ENCODE_ID_NUM_em; i++) {
        if (!(device_is_ready(s_encoder_device_pst[i].encoder_device_pst))) {
            LOG_ERR("Encode %d device not ready",i);
            return -ENODEV;
        }

        //获取原始计数值
        struct sensor_value val;
        sensor_sample_fetch(s_encoder_device_pst[i].encoder_device_pst);
        sensor_channel_get(s_encoder_device_pst[i].encoder_device_pst, SENSOR_CHAN_ENCODER_COUNT, &val);

        s_encoder_device_pst[i].last_raw_l =(int16_t)val.val1;
        s_encoder_device_pst[i].data_st.position_l = 0;
        s_encoder_device_pst[i].data_st.rpm_f = 0.0f;
        
        s_encoder_device_pst->last_tick_ul = k_uptime_get_32();
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

    struct sensor_value val_st;
    int ret;

    ret = sensor_sample_fetch(s_encoder_device_pst[id_em].encoder_device_pst);
    if (ret != 0) {
        LOG_ERR("Encoder %d sample fetch failed: %d",id_em,ret);
        return ret;
    }



    ret = sensor_channel_get(s_encoder_device_pst[id_em].encoder_device_pst,SENSOR_CHAN_ENCODER_COUNT  ,&val_st);

    if (ret != 0) {
        LOG_ERR("Encoder %d channel get failed : %d",id_em,ret);
        return ret;
    }

    uint32_t current_tick_ul = k_uptime_get_32();



    int32_t current_raw_l = val_st.val1;     /* -32768 ~ 32767 */
    int32_t last_raw_l = s_encoder_device_pst[id_em].last_raw_l ;

    int32_t delta_l = current_raw_l - last_raw_l;

    if (delta_l > 60000) {
        delta_l -= 0xFFFF ;
        delta_l += 20;                      //为什么传感器有这样的毛病，需要加一个20进行补偿？
    }
    else if (delta_l < -60000) {
        delta_l += 0xFFFF;
    }
    
    uint32_t dt_ms = current_tick_ul - s_encoder_device_pst[id_em].last_tick_ul;


    //计数器自增
    s_encoder_device_pst[id_em].data_st.position_l  += delta_l;
    s_encoder_device_pst[id_em].last_raw_l = current_raw_l;
    s_encoder_device_pst[id_em].last_tick_ul = current_tick_ul;
    
    if (dt_ms > 0) {
        s_encoder_device_pst[id_em].data_st.rpm_f = 
        ((float)delta_l / dt_ms) * 10.0f;}
    return 0;
}

void encoder_update_all(void)
{
    for (int i = 0; i < ENCODE_ID_NUM_em; i++) {
        encoder_read(i);
    }
    menu_request_refresh(g_encode_oled_pst);
}


int encoder_get_data(enum encoder_id_e id_em, struct encoder_data_t* out)
{
     if (id_em >= ENCODE_ID_NUM_em) {
        return -ENODEV;
    }
    *out = s_encoder_device_pst[id_em].data_st;
    return 0;
}

