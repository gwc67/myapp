/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <arm_acle.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/display/cfb.h>
#include <zephyr/sys/printk.h>
#include "ano/ano_base.h"
#include "ano/ano_device/ano_device_com.h"
#include "encode/encode.h"
#include "menu/menu.h"
#include "ano.h"
#include "mess/value_to_str.h"
#include "motor/tb6612.h"
#include "mpu6050/ahrs_madgwick.h"
#include "mpu6050/euler.h"
#include "zephyr/kernel/thread.h"
#include "zephyr/kernel/thread_stack.h"
#include "zephyr/syscalls/kernel.h"


/* 1ms 任务：euler_update() 做 AHRS 解算，可能有浮点运算 */
static K_THREAD_STACK_DEFINE(s_stack_1ms_high, 2048);   /* 2KB 足够 */

/* 1ms 低优先级任务：简单任务 */
static K_THREAD_STACK_DEFINE(s_stack_1ms_low, 1024);   /* 1KB */

/* 10ms 任务：ano_check_data + com_check_to_send + menu */
static K_THREAD_STACK_DEFINE(s_stack_10ms_high, 1536); /* 1.5KB */

/* 10ms 低优先级任务 */
static K_THREAD_STACK_DEFINE(s_stack_10ms_low, 1024);    /* 1KB */

static K_THREAD_STACK_DEFINE(s_stack_100ms_high, 512);    /* 1KB */

static struct k_thread s_thread_1ms_high;
static struct k_thread s_thread_1ms_low;
static struct k_thread s_thread_10ms_high;
static struct k_thread s_thread_10ms_low;
static struct k_thread s_thread_100ms_high;

static void s_task_1ms_high(void *p1,void *p2,void *p3)
{
	while (1) {
		euler_update();
		ano_check_data(g_com_ano_pst);
		k_msleep(1);
	}
}

static void s_task_1ms_low(void *p1,void *p2,void *p3)
{
	while (1) {
		
		com_check_to_send();
		k_msleep(1);
	}
}

static void s_task_10ms_high(void *p1,void *p2,void *p3)
{
	while (1) {

		encoder_update_all();
		k_msleep(10);

	}
}

static void s_task_10ms_low(void *p1,void *p2,void *p3)
{
	while (1) {
		menu_task_v();
		k_msleep(10);

	}
}

static void s_task_100ms_high(void *p1,void *p2,void *p3)
{
	while (1) {
		
		
		// printk("encode_a : pos %d , rpm = %s \r\n",encode_a_st.position_l,num);

		static int16_t s_speed_a_s = 100;
		motor_set(g_motor_a_pst, s_speed_a_s);
		motor_set(g_motor_b_pst, s_speed_a_s);
		k_msleep(1000);
	}
}




int main(void)
{

	k_thread_create(&s_thread_1ms_high, s_stack_1ms_high, sizeof(s_stack_1ms_high), s_task_1ms_high, NULL, NULL, NULL, K_PRIO_PREEMPT(2), 0, K_NO_WAIT);
	k_thread_create(&s_thread_1ms_low,  s_stack_1ms_low , sizeof(s_stack_1ms_low) , s_task_1ms_low , NULL, NULL, NULL, K_PRIO_PREEMPT(5), 0, K_NO_WAIT);
	k_thread_create(&s_thread_10ms_high, s_stack_10ms_high, sizeof(s_stack_10ms_high), s_task_10ms_high, NULL, NULL, NULL, K_PRIO_PREEMPT(4), 0, K_NO_WAIT);
	k_thread_create(&s_thread_10ms_low,  s_stack_10ms_low , sizeof(s_stack_10ms_low) , s_task_10ms_low , NULL, NULL, NULL, K_PRIO_PREEMPT(10), 0, K_NO_WAIT);
	k_thread_create(&s_thread_100ms_high,  s_stack_100ms_high , sizeof(s_stack_100ms_high) , s_task_100ms_high , NULL, NULL, NULL, K_PRIO_PREEMPT(11), 0, K_NO_WAIT);
	while (1) {
		k_sleep(K_MSEC(1000));
	}
	return 0;
}

