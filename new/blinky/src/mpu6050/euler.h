#ifndef  __EULER_H
#define  __EULER_H



// struct euler_t{
//     double pitch_db;
//     double roll_db;
//     double yaw_db;
// };

// struct Kalman_t
// {
//     double q_angle_db;  //状态向量 X = [angle , bias]^t angle 角度估计值 bias 陀螺仪零偏估计值
//     double q_bias_db;   
//     double r_measure_db;    
//     double angle_db;
//     double bias_db;
//     double P_pdb[2][2]; //协方差矩阵
// };


// void euler_copy(struct euler_t* out);
void euler_update(void);
extern float angle_acc_f;


#endif
