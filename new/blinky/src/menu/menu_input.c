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
struct key_mapping {
	uint16_t code;                     /* Zephyr input code, e.g. INPUT_KEY_0 */
	enum menu_action_e single;         /* 单击动作 */
	enum menu_action_e dbl;            /* 双击动作 (0xFF = 无) */
	enum menu_action_e lng;            /* 长按动作 (0xFF = 无) */
};

#define NO_ACTION  0xFF                /* 哨兵: 该手势无对应动作 */

static const struct key_mapping key_map[] = {
	{ INPUT_KEY_0,  MENU_ACTION_UP,      NO_ACTION,       NO_ACTION },
	{ INPUT_KEY_1,  MENU_ACTION_DOWN,    NO_ACTION,       NO_ACTION },
	{ INPUT_KEY_2,  MENU_ACTION_ENTER,   MENU_ACTION_TOGGLE, MENU_ACTION_RESET },
	{ INPUT_KEY_3,  MENU_ACTION_BACK,    NO_ACTION,       NO_ACTION },
};

/* ---- 状态 ---- */
static menu_action_handler_t s_handler;

/* 双击检测 */
static uint32_t s_last_press_time;
static uint16_t s_last_press_code;

/* 长按检测 */
static bool     s_is_holding;
static uint32_t s_held_start_time;
static uint16_t s_held_code;
static bool     s_long_fired;

/* ---- 内部: 按键码 → 动作查找 ---- */
static const struct key_mapping *find_mapping(uint16_t code)
{
	for (int i = 0; i < sizeof(key_map) / sizeof(key_map[0]); i++) {
		if (key_map[i].code == code)
			return &key_map[i];
	}
	return NULL;
}

/* ---- 内部: 向 menu.c 发送动作 ---- */
static void notify(enum menu_action_e action)
{
	if (action != NO_ACTION && s_handler)
		s_handler(action);
}

/* ---- Zephyr input 原始回调 ---- */
static void raw_input_cb(struct input_event *evt, void *user_data)
{
	bool pressed = (evt->value == 1);
	uint32_t now = k_uptime_get_32();

	if (pressed) {
		const struct key_mapping *map = find_mapping(evt->code);
		if (!map) return;

		/* 双击检测: 同键 400ms 内再按 */
		if (evt->code == s_last_press_code
		    && (now - s_last_press_time) < DOUBLE_CLICK_MS) {
			notify(map->dbl);
			s_last_press_time = 0;   /* 清零防三击 */
			s_is_holding = false;     /* 双击不触发长按 */
			return;
		}

		/* 单击: 立即触发 */
		notify(map->single);

		/* 记录, 等双击 */
		s_last_press_time = now;
		s_last_press_code = evt->code;

		/* 开始长按计时 */
		s_is_holding = true;
		s_held_start_time = now;
		s_held_code = evt->code;
		s_long_fired = false;

	} else {
		/* 释放: 清除长按状态 */
		s_is_holding = false;
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
	s_handler = handler;
}

void menu_input_poll(void)
{
	if (!s_is_holding || s_long_fired) return;

	uint32_t now = k_uptime_get_32();
	if ((now - s_held_start_time) < LONG_PRESS_MS) return;

	/* 长按触发 */
	const struct key_mapping *map = find_mapping(s_held_code);
	if (map) {
		notify(map->lng);
	}
	s_long_fired = true;
}
