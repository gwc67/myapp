/*
 * 菜单输入中间层 — 实现
 *
 * 手势识别:
 *   - 单击: 按键按下时立即触发 (无延迟)
 *   - 双击: 同一键在 400ms 内再次按下 → 触发双击动作 (对应按键的 TOGGLE)
 *   - 长按: 按住超过 800ms → 在 menu_input_poll() 中触发 (对应按键的 RESET)
 *
 * 按键 → 动作映射表 (key_map[]), 改按键布局只改这一个表
 */

#include "menu_input.h"

#include <zephyr/input/input.h>
#include <zephyr/kernel.h>

/* ---- 手势阈值 ---- */
#define DOUBLE_CLICK_MS  400
#define LONG_PRESS_MS    800

/* ---- 按键到动作的映射 ---- */
struct key_mapping_t {
	uint16_t code_us;                     /* Zephyr input code, e.g. INPUT_KEY_0 */
	enum menu_action_e single_em;         /* 单击动作 */
	enum menu_action_e dbl_em;            /* 双击动作 (0xFF = 无) */
	enum menu_action_e lng_em;            /* 长按动作 (0xFF = 无) */
};

#define NO_ACTION  0xFF                /* 哨兵: 该手势无对应动作 */

static const struct key_mapping_t s_key_map_pst[] = {
	{ INPUT_KEY_0,  MENU_ACTION_UP_em,      NO_ACTION,       NO_ACTION },
	{ INPUT_KEY_1,  MENU_ACTION_DOWN_em,    NO_ACTION,       NO_ACTION },
	{ INPUT_KEY_2,  MENU_ACTION_ENTER_em,   MENU_ACTION_TOGGLE_em, MENU_ACTION_RESET_em },
	{ INPUT_KEY_3,  MENU_ACTION_BACK_em,    NO_ACTION,       NO_ACTION },
};

/* ---- 状态 ---- */
static menu_action_handler_t s_handler_st;

/* 双击检测 */
static uint32_t s_last_press_time_ul;
static uint16_t s_last_press_code_us;

/* 长按检测 */
static bool     s_is_holding_b;
static uint32_t s_held_start_time_ul;
static uint16_t s_held_code_us;
static bool     s_long_fired_b;

/* ---- 内部: 按键码 → 动作查找 ---- */
static const struct key_mapping_t *find_mapping(uint16_t code)
{
	for (int i = 0; i < sizeof(s_key_map_pst) / sizeof(s_key_map_pst[0]); i++) {
		if (s_key_map_pst[i].code_us == code)
			return &s_key_map_pst[i];
	}
	return NULL;
}

/* ---- 内部: 向 menu.c 发送动作 ---- */
static void notify(enum menu_action_e action)
{
	if (action != NO_ACTION && s_handler_st)
		s_handler_st(action);
}

/* ---- Zephyr input 原始回调 ---- */
static void raw_input_cb(struct input_event *evt, void *user_data)
{
	bool pressed_b = (evt->value == 1);
	uint32_t now_ul = k_uptime_get_32();

	if (pressed_b) {
		const struct key_mapping_t *c_map_pst = find_mapping(evt->code);
		if (!c_map_pst) return;

		/* 双击检测: 同键 400ms 内再按 */
		if (evt->code == s_last_press_code_us
		    && (now_ul - s_last_press_time_ul) < DOUBLE_CLICK_MS) {
			notify(c_map_pst->dbl_em);
			s_last_press_time_ul = 0;   /* 清零防三击 */
			s_is_holding_b = false;     /* 双击不触发长按 */
			return;
		}

		/* 单击: 立即触发 */
		notify(c_map_pst->single_em);

		/* 记录, 等双击 */
		s_last_press_time_ul = now_ul;
		s_last_press_code_us = evt->code;

		/* 开始长按计时 */
		s_is_holding_b = true;
		s_held_start_time_ul = now_ul;
		s_held_code_us = evt->code;
		s_long_fired_b = false;

	} else {
		/* 释放: 清除长按状态 */
		s_is_holding_b = false;
	}
}

INPUT_CALLBACK_DEFINE(NULL, raw_input_cb, NULL);

/* ---- 公开 API ---- */

void menu_input_init(void)
{
	/* INPUT_CALLBACK_DEFINE 是编译期注册, 无需运行时初始化 */
}

void menu_input_register_handler(menu_action_handler_t handler)
{
	s_handler_st = handler;
}

void menu_input_poll(void)
{
	if (!s_is_holding_b || s_long_fired_b) return;

	uint32_t now = k_uptime_get_32();
	if ((now - s_held_start_time_ul) < LONG_PRESS_MS) return;

	/* 长按触发 */
	const struct key_mapping_t *map = find_mapping(s_held_code_us);
	if (map) {
		notify(map->lng_em);
	}
	s_long_fired_b = true;
}
