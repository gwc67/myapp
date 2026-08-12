/*
 * 菜单输入中间层 — 实现 (全中断驱动, 零轮询)
 *
 * ── 与 Linux input 子系统的对应 ──
 *   Zephyr INPUT_CALLBACK_DEFINE  →  Linux evdev 原始事件
 *   k_work_delayable              →  Linux timer + workqueue
 *   notify(action)                →  应用层消费语义事件
 *
 * 手势识别:
 *   - 单击: 无双击动作的键 → 按下立即触发
 *          有双击动作的键 → 延迟 400ms (用 k_work), 窗口内无第二次按下才触发
 *   - 双击: 窗口内同键再按 → 取消延迟单击, 触发双击
 *   - 长按: 按下后 800ms 仍按住 → k_work 触发长按动作; 提前释放则取消
 */

#include "menu_input.h"
#include "system_stm32f4xx.h"
#include "zephyr/init.h"

#include <sys/_intsup.h>
#include <zephyr/input/input.h>
#include <zephyr/kernel.h>

/* ---- 手势阈值 ---- */
#define DOUBLE_CLICK_MS  400
#define LONG_PRESS_MS    800

/* ---- 按键 → 动作映射 ---- */
struct key_mapping_t {
	uint16_t code_us;
	enum menu_action_e single_em;
	enum menu_action_e dbl_em;
	enum menu_action_e lng_em;
};

#define NO_ACTION  ((enum menu_action_e)0xFF)

static const struct key_mapping_t s_key_map_pst[] = {
	{ INPUT_KEY_0,  MENU_ACTION_UP_em,      NO_ACTION,       NO_ACTION },
	{ INPUT_KEY_1,  MENU_ACTION_DOWN_em,    NO_ACTION,       NO_ACTION },
	{ INPUT_KEY_2,  MENU_ACTION_ENTER_em,   MENU_ACTION_TOGGLE_em, MENU_ACTION_RESET_em },
	{ INPUT_KEY_3,  MENU_ACTION_BACK_em,    NO_ACTION,       NO_ACTION },
};

/* ---- 状态 ---- */
static menu_action_handler_t s_handler_st;

/* 延迟单击 work — 等价于 Linux timer */
static struct k_work_delayable s_single_dwork;
static const struct key_mapping_t *s_single_map_pst;

/* 长按 work */
static struct k_work_delayable s_long_dwork;
static const struct key_mapping_t *s_long_map_pst;

/* ---- 内部 ---- */
static const struct key_mapping_t *find_mapping(uint16_t code)
{
	for (int i = 0; i < sizeof(s_key_map_pst) / sizeof(s_key_map_pst[0]); i++) {
		if (s_key_map_pst[i].code_us == code)
			return &s_key_map_pst[i];
	}
	return NULL;
}

static void notify(enum menu_action_e action)
{
	if (action != NO_ACTION && s_handler_st)
		s_handler_st(action);
}

/* ---- work 回调 (系统 workqueue 上下文, 非中断) ---- */

static void single_work_cb(struct k_work *work)
{
	notify(s_single_map_pst->single_em);
	s_single_map_pst = NULL;  /* 清除, 否则下次按下会误判为双击 */
}

static void long_work_cb(struct k_work *work)
{
	notify(s_long_map_pst->lng_em);
}

/* ---- Zephyr input 原始回调 (中断上下文) ---- */

static void raw_input_cb(struct input_event *evt, void *user_data)
{
	bool pressed_b = (evt->value == 1);
	const struct key_mapping_t *c_map_pst = find_mapping(evt->code);

	if (!c_map_pst) return;

	if (pressed_b) {

		/* ── 双击检测: 延迟单击尚未触发, 同键再次按下 ── */
		if (s_single_map_pst == c_map_pst
		    && k_work_cancel_delayable(&s_single_dwork) == 0) {
			/* 取消延迟单击成功 → 这是双击 */
			k_work_cancel_delayable(&s_long_dwork);
			s_single_map_pst = NULL;
			notify(c_map_pst->dbl_em);
			/* 双击后重新开始长按计时 */
			if (c_map_pst->lng_em != NO_ACTION) {
				s_long_map_pst = c_map_pst;
				k_work_schedule(&s_long_dwork,
				                K_MSEC(LONG_PRESS_MS));					//计时LONG_PRESS_MS 后执行s_long_dwork函数 这个函数和 long_work_cb 是绑定在一起的
			}
			return;
		}

		/* ── 单击: 有双击动作 → 延迟; 无 → 立即 ── */
		if (c_map_pst->dbl_em != NO_ACTION) {
			/* 取消上一个挂起的单击 (不同键快速先后按下) */
			k_work_cancel_delayable(&s_single_dwork);
			s_single_map_pst = c_map_pst;
			k_work_schedule(&s_single_dwork,
			                K_MSEC(DOUBLE_CLICK_MS));					//等待DOUBLE_CLICK_MS 后 触发单击，如果双击检测成功，就会取消单击事件，从而触发双击事件
		} else {
			notify(c_map_pst->single_em);
		}

		/* ── 长按计时 ── */
		if (c_map_pst->lng_em != NO_ACTION) {
			s_long_map_pst = c_map_pst;
			k_work_schedule(&s_long_dwork,
			                K_MSEC(LONG_PRESS_MS));
		}

	} else {
		/* 释放: 取消长按计时 */
		k_work_cancel_delayable(&s_long_dwork);
	}
}

INPUT_CALLBACK_DEFINE(NULL, raw_input_cb, NULL);

/* ---- 公开 API ---- */

int menu_input_init(void)
{
	k_work_init_delayable(&s_single_dwork, single_work_cb);
	k_work_init_delayable(&s_long_dwork, long_work_cb);
	return 0;
	
}

//OLED 底层初始化
SYS_INIT(menu_input_init, APPLICATION, 11);

void menu_input_register_handler(menu_action_handler_t handler)
{
	s_handler_st = handler;
}
