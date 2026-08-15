#include "euler.h"
#include "menu.h"
#include "mpu6050.h"
#include "zephyr/kernel.h"
#include <math.h>
#include <stdint.h>
#include "OLED_Menu.h"

#define RAD_TO_DEG 57.295779513082320876798154814105


static struct euler_t s_euler_st = {0};

static struct Kalman_t s_K_pitch_st = {
    .q_angle_db = 0.001,
    .q_bias_db = 0.003,
    .r_measure_db = 0.03,
};
static struct Kalman_t s_K_roll_st = {
    .q_angle_db = 0.001,
    .q_bias_db = 0.003,
    .r_measure_db = 0.03,
};
static struct Kalman_t s_K_yaw_st = {
    .q_angle_db = 0.001,
    .q_bias_db = 0.003,
    .r_measure_db = 0.03,
};

double kalman_get_angle(struct Kalman_t* kalman_pst,double angle_new_db,double rate_new_db,double dt_db)
{
    double rate_db = rate_new_db - kalman_pst->bias_db;
    kalman_pst->angle_db += dt_db* rate_db;

    kalman_pst->P_pdb[0][0] += dt_db * (dt_db * kalman_pst->P_pdb[1][1] - kalman_pst->P_pdb[0][1] - kalman_pst->P_pdb[1][0] + kalman_pst->q_angle_db);
    kalman_pst->P_pdb[0][1] -= dt_db * kalman_pst->P_pdb[1][1];
    kalman_pst->P_pdb[1][0] -= dt_db * kalman_pst->P_pdb[1][1];
    kalman_pst->P_pdb[1][1] += kalman_pst->q_bias_db * dt_db;

    //卡尔曼增益
    double s_db = kalman_pst->P_pdb[0][0] + kalman_pst->r_measure_db;
    double k_pdb[2];
    k_pdb[0] = kalman_pst->P_pdb[0][0] / s_db;
    k_pdb[1] = kalman_pst->P_pdb[1][0] / s_db;

    double gap_db = angle_new_db - kalman_pst->angle_db;

    kalman_pst->angle_db += k_pdb[0] * gap_db;
    kalman_pst->bias_db += k_pdb[1] * gap_db;

    //更新协方差矩阵
    double p00_temp_db = kalman_pst->P_pdb[0][0];
    double p01_temp_db = kalman_pst->P_pdb[0][1];

    kalman_pst->P_pdb[0][0] -= k_pdb[0] * p00_temp_db;
    kalman_pst->P_pdb[0][1] -= k_pdb[0] * p01_temp_db;
    kalman_pst->P_pdb[1][0] -= k_pdb[1] * p00_temp_db;
    kalman_pst->P_pdb[1][1] -= k_pdb[1] * p01_temp_db;
    
    return kalman_pst->angle_db;
}

void euler_update(void)
{
    mpu6050_sample();

    static uint32_t s_last_tick_ul;
    
    struct mpu6050_vec3_t acc_st = {0};
    struct mpu6050_vec3_t gyro_st = {0};

    mpu6050_get_accel(&acc_st);
    mpu6050_get_gyro(&gyro_st);

    double roll_db,pitch_db,yaw_db = 0;

    double dt_db = (double)(k_uptime_get() - s_last_tick_ul) / 1000;

    double roll_sqrt_db = sqrt(acc_st.x_db * acc_st.x_db + acc_st.z_db * acc_st.z_db);

    //采用ZYX 旋转计算，加速度这个只能够计算静态
    
    if (roll_sqrt_db != 0.0) {
        roll_db = atan(acc_st.y_db / roll_sqrt_db) * RAD_TO_DEG;
    }
    else {
        roll_db = 0.0;
    }

    pitch_db = atan2(acc_st.x_db , acc_st.z_db) * RAD_TO_DEG;

    if ((pitch_db < -90 && s_euler_st.pitch_db > 90) || (pitch_db > 90 && s_euler_st.pitch_db < -90)) {
        s_K_pitch_st.angle_db = pitch_db;
        s_euler_st.pitch_db = pitch_db;
    }
    else {
        s_euler_st.pitch_db = kalman_get_angle(&s_K_pitch_st, pitch_db, gyro_st.y_db, dt_db);
    }

    s_euler_st.roll_db = kalman_get_angle(&s_K_roll_st, roll_db, gyro_st.x_db, dt_db); 

    yaw_db +=  gyro_st.z_db * dt_db;

    s_euler_st.yaw_db = yaw_db;

    menu_request_refresh(g_mpu6050_euler_oled_pst);

}

void euler_copy(struct euler_t* out)
{
    *out = s_euler_st;
}
