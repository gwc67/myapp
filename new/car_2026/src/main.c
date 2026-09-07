#include "zephyr/device.h"
#include "zephyr/drivers/gpio.h"
#include "zephyr/kernel.h"

static K_THREAD_STACK_DEFINE(s_stack_5ms_high, 4096); /* 1.5KB */
static struct k_thread s_thread_5ms_high;

static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0),gpios);


void s_task_5ms_high(void *p1,void *p2,void *p3 )
{
    k_sleep(K_MSEC(1000));
}

int main(void)
{

    k_thread_create(&s_thread_5ms_high, s_stack_5ms_high, sizeof(s_stack_5ms_high), s_task_5ms_high, NULL, NULL, NULL, K_PRIO_PREEMPT(4), 0, K_NO_WAIT);
    gpio_pin_set_dt(&led0, 0);
    k_sleep(K_MSEC(1000));
    gpio_pin_set_dt(&led0, 1);
    while (1) {

	}
	return 0;
}