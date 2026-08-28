#include "debug.h"
#include "blinky.h"
#include "my_ring.h"
#include <stdint.h>
#include <string.h>
#include "value_to_str.h"
#include "zephyr/drivers/gpio.h"
#define LINE_BUF_SIZE 128


static void  dispatch_line(char* line_pc);

static const struct gpio_dt_spec led_motor_st =GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);


void debug_par_check(void)
{
    static char s_line_buf_pc[LINE_BUF_SIZE];
    uint8_t byte;         
    uint8_t lie_pos_uc = 0;
    while (!my_ring_buf_is_empty(g_ut2_rx_ring_pst)) {
        my_ring_buf_get(g_ut2_rx_ring_pst, &byte, 1);
        if (byte == '\n' || byte == '\r') {
            if (lie_pos_uc > 0) {
                s_line_buf_pc[lie_pos_uc] = '\0';
                dispatch_line(s_line_buf_pc);
                lie_pos_uc = 0;
            }
            continue;  // 换行符本身不入缓冲区
        }
        if (lie_pos_uc < LINE_BUF_SIZE - 1) {
            s_line_buf_pc[lie_pos_uc++] = (char)byte;
        } else {
            lie_pos_uc = 0;
        }
    }
}


static void  dispatch_line(char* line_pc)
{
    float values_pf[4] = {0};
    // int16_t values_ps[4] = {0};
    str_to_float(line_pc, values_pf, 4);

    if (strncmp(line_pc, "spd_kff:", 8) == 0) {
        // str_to_float(line_pc, values_pf, 4);
        spd_kff = values_pf[0];
    }
    else if (strncmp(line_pc, "bal_kp_factor:", 14) == 0 ) 
    {
        // str_to_float(line_pc, values_pf, 4);
        bal_kp_factor = values_pf[0];
    }
    else if (strncmp(line_pc, "bal_ki_factor:", 14)== 0 ) 
    {
        bal_ki_factor = values_pf[0];
    }
    else if (strncmp(line_pc, "bal_kd_factor:", 14) == 0) 
    {
        // str_to_float(line_pc, values_pf, 4);
        bal_kd_factor = values_pf[0];
    }
    else if(strncmp(line_pc, "bal_kp:",6) == 0)
    {
        // str_to_float(line_pc, values_pf, 4);
        BALANCE_KP = values_pf[0];
    }
    else if (strncmp(line_pc, "bal_kd:", 6) == 0) {
        // str_to_float(line_pc, values_pf, 4);
        BALANCE_KD = values_pf[0];
    }
    else if (strncmp(line_pc, "spd_kp:", 6) == 0) {
        // str_to_float(line_pc, values_pf, 4);
        SPD_KP = values_pf[0];
    }
    else if (strncmp(line_pc, "spd_ki:", 6) == 0) {
        // str_to_float(line_pc, values_pf, 4);
        SPD_KI = values_pf[0];
    }
    else if (strncmp(line_pc, "tar_spd:", 7) == 0) {
        // str_to_float(line_pc, values_pf, 4);
        rtU.target_speed = values_pf[0];
    }
    else if(strncmp(line_pc,"turn_kp:",7) == 0)
    {
        // str_to_float(line_pc, values_pf, 4);
        TURN_KP = values_pf[0];
    }
    else if(strncmp(line_pc,"turn_kd:",7) == 0)
    {
        // str_to_float(line_pc, values_pf, 4);
        TURN_KD = values_pf[0];
    }
    else if(strncmp(line_pc,"spd_kd:",7) == 0)
    {
        // str_to_float(line_pc, values_pf, 4);
        SPD_KD  = values_pf[0];
    }
    // else if(strncmp(line_pc,"bal_kff:",6) == 0)
    // str_to_float(line_pc,values_pf,4);
    // {
        // BAL_KFF = (double)values_pf[0];
    // }
    else if (strncmp(line_pc, "convert:", 8) == 0) {
        rtU.running_flag ^=1;
        gpio_pin_set_dt(&led_motor_st, rtU.running_flag);
    }
    else if (strncmp(line_pc, "turn_tar:", 9) == 0) {
        // str_to_float(line_pc, values_pf, 4);
        rtU.turn_target = values_pf[0];
        rtU.target_speed =values_pf[1];
    }
    

}
