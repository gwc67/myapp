#include "euler.h"
#include "blinky.h"
#include "menu.h"
#include "mpu6050.h"
#include "zephyr/kernel.h"
#include <math.h>
#include <stdint.h>
#include "OLED_Menu.h"

#define RAD_TO_DEG 57.295779513082320876798154814105


// static struct euler_t s_euler_st = {0};

// static struct Kalman_t s_K_pitch_st = {
//     .q_angle_db = 0.001,
//     .q_bias_db = 0.003,
//     .r_measure_db = 0.03,
// };
// static struct Kalman_t s_K_roll_st = {
//     .q_angle_db = 0.001,
//     .q_bias_db = 0.003,
//     .r_measure_db = 0.03,
// };
// static struct Kalman_t s_K_yaw_st = {
//     .q_angle_db = 0.001,
//     .q_bias_db = 0.003,
//     .r_measure_db = 0.03,
// };


//rate_new_db : U  输入：陀螺仪测量的角度 
//angle_new_db : Z 作为观察 即 加速计计算的角度
//A: 状态转移矩阵 [[1,-d],[0,1]]
//B: 控制矩阵 [dt,0]^t
//H: 观测矩阵[1,0];
// double kalman_get_angle(struct Kalman_t* kalman_pst,double angle_new_db,double rate_new_db,double dt_db)
// {
//     //状态预测 
//     // 新的角度 = 旧角度 + 角度变化 = angle + (rate - bias) × dt
//     //估算角度 
//     //  状态预测：用陀螺仪推算下一时刻角度
//     // 对应公式X̂ₖ⁻ = A·X̂ₖ₋₁ + B·Uₖ₋₁
//     double rate_db = rate_new_db - kalman_pst->bias_db;
//     kalman_pst->angle_db += dt_db* rate_db;

//     // 这是步骤2：协方差预测，对应标准公式：
//     // P_k^- = A \cdot P_{k-1} \cdot A^T + Q
//     kalman_pst->P_pdb[0][0] += dt_db * (dt_db * kalman_pst->P_pdb[1][1] - kalman_pst->P_pdb[0][1] - kalman_pst->P_pdb[1][0] + kalman_pst->q_angle_db);      //angle 的方差（角度估计的不确定性）
//     kalman_pst->P_pdb[0][1] -= dt_db * kalman_pst->P_pdb[1][1];
//     kalman_pst->P_pdb[1][0] -= dt_db * kalman_pst->P_pdb[1][1];
//     kalman_pst->P_pdb[1][1] += kalman_pst->q_bias_db * dt_db;  //bias 的方差（零偏估计的不确定性）定值0.003

//     //卡尔曼增益
//     double s_db = kalman_pst->P_pdb[0][0] + kalman_pst->r_measure_db;
//     double k_pdb[2];
//     k_pdb[0] = kalman_pst->P_pdb[0][0] / s_db;
//     k_pdb[1] = kalman_pst->P_pdb[1][0] / s_db;

//     double gap_db = angle_new_db - kalman_pst->angle_db;

//     kalman_pst->angle_db += k_pdb[0] * gap_db;
//     kalman_pst->bias_db += k_pdb[1] * gap_db;

//     //更新协方差矩阵
//     double p00_temp_db = kalman_pst->P_pdb[0][0];
//     double p01_temp_db = kalman_pst->P_pdb[0][1];

//     kalman_pst->P_pdb[0][0] -= k_pdb[0] * p00_temp_db;
//     kalman_pst->P_pdb[0][1] -= k_pdb[0] * p01_temp_db;
//     kalman_pst->P_pdb[1][0] -= k_pdb[1] * p00_temp_db;
//     kalman_pst->P_pdb[1][1] -= k_pdb[1] * p01_temp_db;
    
//     return kalman_pst->angle_db;
// }

void euler_update(void)
{
    mpu6050_sample();

    // static uint32_t s_last_tick_ul;
    
    struct mpu6050_vec3_t acc_st = {0};
    struct mpu6050_vec3_t gyro_st = {0};

    mpu6050_get_accel(&acc_st);
    mpu6050_get_gyro(&gyro_st);

    // double roll_db,pitch_db;

    // uint32_t now_ms_ul =  k_uptime_get_32();

    // double dt_db = (double)(now_ms_ul - s_last_tick_ul) / 1000;

    // rtU.ax = acc_st.x_db;
    // rtU.ay = acc_st.y_db;
    // rtU.az = acc_st.z_db;

    // rtU.gx = gyro_st.x_db;
    // rtU.gy = gyro_st.y_db;
    // rtU.gz = gyro_st.z_db;

    rtU.gyro[0] = gyro_st.x_db;
    rtU.gyro[1] = gyro_st.y_db;
    rtU.accx = acc_st.x_db;
    rtU.accy = acc_st.y_db;
    rtU.accz = acc_st.z_db;
    

    // s_last_tick_ul = now_ms_ul;
    // double roll_sqrt_db = sqrt(acc_st.x_db * acc_st.x_db + acc_st.z_db * acc_st.z_db);

    // // //采用ZYX 旋转计算，加速度这个只能够计算静态
    // if (roll_sqrt_db != 0.0) {
    //     roll_db = atan(acc_st.y_db / roll_sqrt_db);
    // }
    // else {
    //     roll_db = 0.0;
    // }

    // pitch_db = atan2(-acc_st.x_db , acc_st.z_db);

    
    // rtU.angle_acc[0] = pitch_db;
    // rtU.angle_acc[1] = roll_db;


    // if ((pitch_db < -90 && s_euler_st.pitch_db > 90) || (pitch_db > 90 && s_euler_st.pitch_db < -90)) {
    //     s_K_pitch_st.angle_db = pitch_db;
    //     s_euler_st.pitch_db = pitch_db;
    // }
    // else {
    //     //卡尔曼在这里的本质是为了融合数据，将加速度算出来的 和 陀螺仪 进行融合数据
    //     s_euler_st.pitch_db = kalman_get_angle(&s_K_pitch_st, pitch_db, gyro_st.y_db, dt_db);
    // }

    // s_euler_st.roll_db = kalman_get_angle(&s_K_roll_st, roll_db, gyro_st.x_db, dt_db); 


    // if (fabs(gyro_st.z_db) < 0.011) {
    //     gyro_st.z_db = 0;
    // }
    
    // s_euler_st.yaw_db +=  gyro_st.z_db * 0.005 * RAD_TO_DEG;

}

// void euler_copy(struct euler_t* out)
// {
//     *out = s_euler_st;
// }
