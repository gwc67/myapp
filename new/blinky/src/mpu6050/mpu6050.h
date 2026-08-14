#ifndef __MPU6050_H
#define __MPU6050_H

#include "zephyr/drivers/sensor.h"

struct mpu6050_vec3_t{
    struct sensor_value x_st;
    struct sensor_value y_st;
    struct sensor_value z_st;
} ;

int mpu6050_sample(void);
int mpu6050_get_accel(struct mpu6050_vec3_t* out);
int mpu6050_get_gyro(struct mpu6050_vec3_t *out);
int mpu6050_get_temp(struct sensor_value *out);




#endif


