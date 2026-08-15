#ifndef  __EULER_H
#define  __EULER_H



struct euler_t{
    double pitch_db;
    double roll_db;
    double yaw_db;
};

struct Kalman_t
{
    double q_angle_db;
    double q_bias_db;
    double r_measure_db;
    double angle_db;
    double bias_db;
    double P_pdb[2][2];
};


void euler_copy(struct euler_t* out);
void euler_update(void);



#endif
