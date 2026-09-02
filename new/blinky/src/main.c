/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <arm_acle.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/_intsup.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/display/cfb.h>
#include <zephyr/sys/printk.h>
#include "OLED_Menu.h"
#include "debug/debug.h"
#include "encode/encode.h"
#include "menu/menu.h"
#include "motor/tb6612.h"
#include "mpu6050/ahrs_madgwick.h"
#include "mpu6050/euler.h"
#include "simulink/ARMCortex-M/blinky/blinky.h"
#include "simulink/ARMCortex-M/blinky/rtmodel.h"
#include "uart_base.h"
#include "value_to_str.h"
#include "zephyr/drivers/counter.h"
#include "zephyr/kernel/thread.h"
#include "zephyr/kernel/thread_stack.h"
#include "zephyr/syscalls/kernel.h"
#include "uarts.h"


// static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
#define RAD_TO_DEG 57.295779513082320876798154814105
/* 1ms 任务：euler_update() 做 AHRS 解算，可能有浮点运算 */
static K_THREAD_STACK_DEFINE(s_stack_1ms_high, 2048);   /* 2KB 足够 */

/* 1ms 低优先级任务：简单任务 */
static K_THREAD_STACK_DEFINE(s_stack_10ms_high, 2048);   /* 1KB */

/* 10ms 任务：ano_check_data + com_check_to_send + menu */
static K_THREAD_STACK_DEFINE(s_stack_5ms_high, 4096); /* 1.5KB */

/* 10ms 低优先级任务 */
static K_THREAD_STACK_DEFINE(s_stack_5ms_low, 1024);    /* 1KB */

static K_THREAD_STACK_DEFINE(s_stack_20ms_high, 1024);    /* 1KB */

static K_THREAD_STACK_DEFINE(s_stack_100ms_high, 1024);    /* 1KB */

static struct k_thread s_thread_1ms_high;
static struct k_thread s_thread_10ms_high;
static struct k_thread s_thread_5ms_high;
static struct k_thread s_thread_5ms_low;
static struct k_thread s_thread_20ms_high;
static struct k_thread s_thread_100ms_high;

//信号量定义
static K_SEM_DEFINE(tim5_sem, 0, 1);


static const struct device *tim5_dev_pst;

static void my_1ms_isr(const struct device *dev,void* user_data);


static void my_1ms_isr(const struct device *dev,void* user_data)
{
	k_sem_give(&tim5_sem);
}

int tim5_1ms_init(void)
{
	int ret;
    struct counter_top_cfg top_cfg;
    uint32_t freq;
    uint32_t ticks;

    /* 1. 获取设备 */
    tim5_dev_pst = DEVICE_DT_GET(DT_ALIAS(tim5));
    
    /* 2. 检查设备是否就绪 —— 这是必须的！ */
    if (!device_is_ready(tim5_dev_pst)) {
        printk("TIM5 device not ready!\n");
        return -ENODEV;
    }

	freq = counter_get_frequency(tim5_dev_pst);
	printk("TIM5 frequency: %u Hz\n", freq);

	ticks = counter_us_to_ticks(tim5_dev_pst, 1000);  /* 1000us = 1ms */
    printk("TIM5 1ms ticks: %u\n", ticks);

    /* 5. 配置 top value（非 const，因为 ticks 是动态计算的） */

    top_cfg.ticks = ticks;
    top_cfg.callback = my_1ms_isr;
    top_cfg.user_data = NULL;
    top_cfg.flags = 0;  /* 默认：计数器归零，触发回调 */

	
    ret = counter_start(tim5_dev_pst);
    if (ret) {
        printk("counter_start failed: %d\n", ret);
        return ret;
    }

    /* 7. 设置 top value —— 这会产生周期中断！ */
    ret = counter_set_top_value(tim5_dev_pst, &top_cfg);
    if (ret) {
        printk("counter_set_top_value failed: %d\n", ret);
        return ret;
    }

    printk("TIM5 1ms periodic interrupt started\n");
    return 0;
}

SYS_INIT(tim5_1ms_init, APPLICATION, 10);



static void s_task_1ms_high(void *p1,void *p2,void *p3)
{
	while (1) {

		k_sem_take(&tim5_sem, K_FOREVER);
		blinky_step(0);
		static uint32_t s_last_tick_ul = 0;
		uint32_t current_tick_ul = k_uptime_get_32();
		

		if (current_tick_ul != s_last_tick_ul) {
			s_last_tick_ul = current_tick_ul;
			motor_set(g_motor_b_pst, rtY.motor_b_pwm);
		}
		// ano_check_data(g_com_ano_pst);
	}
}

static void s_task_10ms_high(void *p1,void *p2,void *p3)
{
	while (1) {
		struct encoder_data_t motor_a_st;
		struct encoder_data_t motor_b_st;
		encoder_get_data(g_encoder_a_pst, &motor_a_st);
		encoder_get_data(g_encoder_b_pst, &motor_b_st);
		rtU.motor_a_speed = motor_a_st.rpm_f;
		rtU.motor_b_speed = motor_b_st.rpm_f;	
		blinky_step(2);
		k_msleep(9);
	}
}

static void s_task_5ms_high(void *p1,void *p2,void *p3)
{
	while (1) {

		
		euler_update();
		encoder_update_all();
		blinky_step(1);

		motor_set(g_motor_a_pst, rtY.motor_a_pwm);
		motor_set(g_motor_b_pst, rtY.motor_b_pwm);

		static uint32_t s_last_tick_ul = 0;
		uint32_t current_tick_ul = k_uptime_get_32();
		

		if (current_tick_ul != s_last_tick_ul) {
			s_last_tick_ul = current_tick_ul;
			motor_set(g_motor_b_pst, rtY.motor_b_pwm);
		}
		


		k_msleep(4);

	}
}

static void s_task_5ms_low(void *p1,void *p2,void *p3)
{

	while (1) {
    	menu_request_refresh(g_mpu6050_euler_oled_pst);
		char buf[150];
		
#if SIMULINK_DEBUG

		char accx_pc[10];
		char accy_pc[10];
		char accz_pc[10];
		char gyrox_pc[10];
		char gyroy_pc[10];
		char time_pc[10];
		char pitch_pc[10];
		char target_angle_pc[10];

		float_to_str(accx_pc, sizeof(accx_pc), rtU.accx , 2);
		float_to_str(accy_pc, sizeof(accy_pc), rtU.accy, 2);
		float_to_str(accz_pc, sizeof(accz_pc), rtU.accz , 2);
		float_to_str(gyrox_pc, sizeof(gyrox_pc), rtU.gyrox , 2);
		float_to_str(gyroy_pc, sizeof(gyroy_pc), rtU.gyroy , 2);
		float_to_str(time_pc, sizeof(time_pc), k_uptime_get_32() * 0.001f , 3);
		float_to_str(pitch_pc, sizeof(pitch_pc), rtY.pitch ,2);
		float_to_str(target_angle_pc, sizeof(target_angle_pc), rtY.angle_target ,2);

		snprintf(buf, sizeof(buf), "%s,%s,%s,%s,%s,%s,%d,%d,%s,%d,%s\n",time_pc,accx_pc,accy_pc,accz_pc,gyrox_pc,gyroy_pc,(int32_t)rtU.motor_a_speed,(int32_t)rtU.motor_b_speed,pitch_pc,rtY.motor_a_pwm,target_angle_pc);
		
#else
		char pitch_pc[10];
		char pitch_target_pc[10];
		char target_speed_pc[10];
		char time_pc[10];
		char e_factor_pc[10];
		char ec_factor_pc[10];
		char ec_raw_pc[10];
		char bal_kp_fuzzy_pc[10];
		char bal_ki_fuzzy_pc[10];
		char bal_kd_fuzzy_pc[10];
		char bal_integral_pc[10];
		char speed_pc[10];

		float_to_str(pitch_pc, sizeof(pitch_pc), rtY.pitch , 2);
		float_to_str(target_speed_pc, sizeof(target_speed_pc), rtU.target_speed , 2);
		float_to_str(time_pc, sizeof(time_pc), k_uptime_get_32() * 0.001f , 3);
		float_to_str(pitch_target_pc, sizeof(pitch_target_pc), rtY.angle_target , 2);
		float_to_str(e_factor_pc, sizeof(e_factor_pc), rtY.e_factor , 2);
		float_to_str(ec_factor_pc, sizeof(ec_factor_pc), rtY.ec_factor , 2);
		float_to_str(ec_raw_pc, sizeof(ec_raw_pc), rtY.ec_raw , 2);
		float_to_str(bal_kp_fuzzy_pc, sizeof(bal_kp_fuzzy_pc), rtY.bal_kp_fuzzy , 2);
		float_to_str(bal_ki_fuzzy_pc, sizeof(bal_ki_fuzzy_pc),rtY.bal_ki_fuzzy, 2);
		float_to_str(bal_kd_fuzzy_pc, sizeof(bal_kd_fuzzy_pc),rtY.bal_kd_fuzzy , 2);
		float_to_str(speed_pc, sizeof(speed_pc),rtY.speed_avg_f , 2);
		float_to_str(bal_integral_pc, sizeof(bal_integral_pc),rtY.bal_integral_f , 2);

		snprintf(buf,sizeof(buf),"%s,%s,%s,%d,%s,%s,%d,%s,%s,%s,%s,%s,%s,%s\n",time_pc,pitch_pc,pitch_target_pc,rtY.motor_a_pwm,speed_pc,target_speed_pc,rtY.running_success_flag,e_factor_pc,ec_factor_pc,ec_raw_pc,bal_kp_fuzzy_pc,bal_ki_fuzzy_pc,bal_kd_fuzzy_pc,bal_integral_pc);
		#endif
		uart_transmit(g_uart2_pst, buf, strlen(buf));
		
		menu_task_v();
		k_msleep(4);
	}
}

static void s_task_20_ms_high(void* p1,void *p2,void* p3)
{
	while (1) {

		blinky_step3();
		k_msleep(19);
	}
}

static void s_task_100ms_high(void *p1,void *p2,void *p3)
{
	while (1) {		
		
		debug_par_check();
		k_msleep(99);
	}
}




int main(void)
{
	blinky_initialize();
	k_thread_create(&s_thread_1ms_high, s_stack_1ms_high, sizeof(s_stack_1ms_high), s_task_1ms_high, NULL, NULL, NULL, K_PRIO_PREEMPT(2), 0, K_NO_WAIT);
	k_thread_create(&s_thread_5ms_high, s_stack_5ms_high, sizeof(s_stack_5ms_high), s_task_5ms_high, NULL, NULL, NULL, K_PRIO_PREEMPT(4), 0, K_NO_WAIT);
	k_thread_create(&s_thread_10ms_high,  s_stack_10ms_high , sizeof(s_stack_10ms_high) , s_task_10ms_high , NULL, NULL, NULL, K_PRIO_PREEMPT(5), 0, K_NO_WAIT);
	k_thread_create(&s_thread_20ms_high,  s_stack_20ms_high , sizeof(s_stack_20ms_high) , s_task_20_ms_high , NULL, NULL, NULL, K_PRIO_PREEMPT(6), 0, K_NO_WAIT);
	k_thread_create(&s_thread_5ms_low,  s_stack_5ms_low , sizeof(s_stack_5ms_low) , s_task_5ms_low , NULL, NULL, NULL, K_PRIO_PREEMPT(10), 0, K_NO_WAIT);
	k_thread_create(&s_thread_100ms_high,  s_stack_100ms_high , sizeof(s_stack_100ms_high) , s_task_100ms_high , NULL, NULL, NULL, K_PRIO_PREEMPT(11), 0, K_NO_WAIT);
	while (1) {
		k_sleep(K_MSEC(1000));
	}
	return 0;
}

