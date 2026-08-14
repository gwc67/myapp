#include "mpu6050.h"
#include "zephyr/device.h"
#include "zephyr/drivers/sensor.h"
#include "zephyr/init.h"
#include "zephyr/syscalls/sensor.h"
#include <sys/errno.h>


static const struct device* mpu6050_dev_pst;



static int s_mpu6050_init(void)
{
    mpu6050_dev_pst = DEVICE_DT_GET(DT_NODELABEL(mpu6050));
    if (!device_is_ready(mpu6050_dev_pst)) {
        return -ENODEV;
    }
    return 0;
}
SYS_INIT(s_mpu6050_init,APPLICATION, 10);

int mpu6050_sample(void)
{
    return sensor_sample_fetch(mpu6050_dev_pst);
}

int mpu6050_get_accel(struct mpu6050_vec3_t* out)
{
    struct sensor_value buf[3];
    int ret = sensor_channel_get(mpu6050_dev_pst, SENSOR_CHAN_ACCEL_XYZ, buf);
    if (ret == 0 && out) {
        out->x_st = buf[0];
        out->y_st = buf[1];
        out->z_st = buf[2];
    }
    return ret;
}

int mpu6050_get_gyro(struct mpu6050_vec3_t *out)
{
    struct sensor_value buf[3];
    int ret = sensor_channel_get(mpu6050_dev_pst, SENSOR_CHAN_GYRO_XYZ, buf);
    if (ret == 0 && out) {
        out->x_st = buf[0];
        out->y_st = buf[1];
        out->z_st = buf[2];
    }
    return ret;
}

int mpu6050_get_temp(struct sensor_value *out)
{
    return sensor_channel_get(mpu6050_dev_pst, SENSOR_CHAN_DIE_TEMP, out);
}


