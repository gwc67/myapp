#include "debug.h"
#include "blinky.h"
#include "my_ring.h"
#include <string.h>
#include "uart_base.h"
#include "value_to_str.h"
#include "uarts.h"
#define LINE_BUF_SIZE 128


static void  dispatch_line(char* line_pc);



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
    
    if(strncmp(line_pc, "bal_kp:",6) == 0)
    {
        str_to_float(line_pc, values_pf, 4);
        BALANCE_KP = (double)values_pf[0];
    }
    else if (strncmp(line_pc, "bal_kd:", 6) == 0) {
        str_to_float(line_pc, values_pf, 4);
        BALANCE_KD = (double)values_pf[0];
    }
    else if (strncmp(line_pc, "spd_kp:", 6) == 0) {
        str_to_float(line_pc, values_pf, 4);
        SPD_KP = (double)values_pf[0];
    }
    else if (strncmp(line_pc, "spd_ki:", 6) == 0) {
        str_to_float(line_pc, values_pf, 4);
        SPD_KI = (double)values_pf[0];
    }
    else if (strncmp(line_pc, "tar_spd:", 7) == 0) {
        str_to_float(line_pc, values_pf, 4);
        rtU.target_speed = (double)values_pf[0];
    }
    else if(strncmp(line_pc,"turn_kp:",7) == 0)
    {
        str_to_float(line_pc, values_pf, 4);
        TURN_KP = values_pf[0];
    }
    else if(strncmp(line_pc,"turn_kd:",7) == 0)
    {
        str_to_float(line_pc, values_pf, 4);
        TURN_KD = values_pf[0];
    }
}
