# Drv_Key — GPIO 按键驱动（Linux 风格）

## 概述

GPIO 按键驱动，使用 Linux 内核风格封装。**数据驱动，无 vtable（单一 GPIO 按键实现不需要多态）**。

- **硬件**: PA4~PA7，4 个按键（BACK/UP/DOWN/ENTER）
- **架构**: 硬件描述表 (const) + 运行时状态 (不透明) 分离
- **消抖**: 两阶段状态机（跳变检测 → 等待稳定 → 边沿上报）
- **初始化**: `DRIVER_INIT` 自动注册

## 架构设计

### 为什么不使用 vtable？

与 Linux `gpio_keys` 驱动一致：

| 层级 | 需要 vtable？ | 原因 |
|------|-------------|------|
| Input 设备（Linux） | ✅ | USB HID、GPIO、触摸屏…实现不同 |
| GPIO 按键（Linux） | ❌ | 所有 GPIO 按键行为一样：读引脚→消抖→上报 |
| **本驱动** | ❌ | 只有一种按键硬件，数据驱动足够 |

**如果以后要加 ADC 按键矩阵、I2C 键盘芯片** → 加 vtable 抽象层（类比 Linux `input_dev`）。

### 三层分离

```
┌─────────────────────────────────────────────┐
│  硬件描述表 (const, ROM)                     │
│  struct key_hw_desc → 哪个 GPIO、有效电平     │
├─────────────────────────────────────────────┤
│  运行时状态 (不透明, .c 内部)                 │
│  struct key_runtime → 消抖状态、边沿检测      │
├─────────────────────────────────────────────┤
│  值类型快照 (公开, .h)                        │
│  struct Key_State_t → 调用者获取的按键状态    │
└─────────────────────────────────────────────┘
```

## 硬件映射

| 枚举 | 功能 | GPIO | 有效电平 |
|------|------|------|---------|
| `KEY_BACK_em` | 返回键 | PA4 | 高有效 |
| `KEY_UP_em` | 上键 | PA5 | **低有效** |
| `KEY_DOWN_em` | 下键 | PA6 | 高有效 |
| `KEY_ENTER_em` | 确认键 | PA7 | 高有效 |

**注意**: UP 键是低电平有效（可能是下拉输入，按下接地）。

## 消抖状态机

### 两阶段流程

```
引脚信号:   ____╱╲╱╲╱╲_______________________________╱╲___
                 ↑ 抖动区间 ↑        稳定了              ↑新跳变

阶段①:        检测到跳变 → 重置计时器 → continue（拒绝接受）
阶段②:        计时器 < 20ms → continue（继续等待）
              计时器 ≥ 20ms → 接受新值 → 上报边沿
```

### 关键理解：引脚稳定时发生了什么？

```c
// 阶段①：raw == raw_prev → false → 跳过，不重置计时器
if (raw_b != rt_pst->raw_prev_b) { ... continue; }

// 阶段②：now - edge_tick = 5000ms，5000 < 20 → false → 跳过，不等待
if (now_ms_ul - rt_pst->edge_tick_ms_ul < KEY_DEBOUNCE_MS) { continue; }

// ✅ 直接穿透到状态更新 → 没有"在消抖"，只是走流程
```

**引脚稳定时，两个 `continue` 都不触发。** 计时器早已过期，代码直接更新状态。

### 状态转换图

```
IDLE (稳定)
  │
  ├─ 引脚跳变 → raw_prev != raw
  │    └─ 重置 edge_tick, edge_reported=false → continue
  │
  ├─ 消抖中 → now - edge_tick < 20ms
  │    └─ continue（等待）
  │
  └─ 消抖完成 → now - edge_tick ≥ 20ms
       └─ stable = raw
       └─ if (!edge_reported && stable != prev_stable)
            └─ edge_reported = true
            └─ 上报 just_pressed / just_released
            └─ 触发回调 (如果有)
```

## API

### 核心 API

```c
// 周期扫描（建议 10ms 调用一次）
// now_ms_ul: 时间戳，使用 xTaskGetTickCount() 或 HAL_GetTick()
void key_scan_v(uint32_t now_ms_ul);

// 值快照（Copy 模式，调用者获取所有按键状态）
void key_state_copy_v(struct Key_State_t *dst_pst, uint8_t count_uc);

// 注册事件回调（按下/释放边沿触发）
void key_event_register_v(key_event_callback_t callback_pst);
```

### 值类型：Key_State_t

```c
struct Key_State_t {
    bool pressed_b;         // 当前稳定按下状态（电平）
    bool just_pressed_b;    // 本周期按下边沿（脉冲，只维持一个扫描周期）
    bool just_released_b;   // 本周期释放边沿（脉冲）
};
```

**使用方式**:
- `pressed_b` → 查询"现在是否按着"
- `just_pressed_b` → 检测"刚刚按下"（边沿触发）
- `just_released_b` → 检测"刚刚松开"

## 使用示例

### 1. 轮询方式（检测边沿）

```c
// 在 RTOS 任务中，10ms 周期
void some_task(void *arg) {
    struct Key_State_t keys_st[KEY_NUM_em];
    
    for (;;) {
        key_scan_v(xTaskGetTickCount());
        key_state_copy_v(keys_st, KEY_NUM_em);
        
        if (keys_st[KEY_UP_em].just_pressed_b) {
            // 上键刚按下 → 执行一次
        }
        
        if (keys_st[KEY_ENTER_em].pressed_b) {
            // 确认键一直按着 → 重复执行
        }
        
        osDelay(10);
    }
}
```

### 2. 事件驱动方式（回调）

```c
static void my_key_handler(enum Key_Id_e id_em, enum Key_Event_e event_em)
{
    if (id_em == KEY_ENTER_em && event_em == KEY_EVENT_PRESSED_em) {
        // 确认键按下 → 触发某个动作
    }
    
    if (id_em == KEY_UP_em && event_em == KEY_EVENT_RELEASED_em) {
        // 上键松开 → 触发另一个动作
    }
}

// 初始化时注册（只需一次）
void some_init(void) {
    key_event_register_v(my_key_handler);
}
```

### 3. 组合使用

```c
// 回调处理"一次性事件"（如菜单确认）
// 轮询处理"持续状态"（如长按加速）

static void key_callback(enum Key_Id_e id_em, enum Key_Event_e event_em) {
    if (id_em == KEY_ENTER_em && event_em == KEY_EVENT_PRESSED_em) {
        menu_confirm();  // 按下瞬间触发
    }
}

void menu_task(void *arg) {
    struct Key_State_t keys_st[KEY_NUM_em];
    
    for (;;) {
        key_scan_v(xTaskGetTickCount());
        key_state_copy_v(keys_st, KEY_NUM_em);
        
        // 长按 UP 键 → 连续增加
        if (keys_st[KEY_UP_em].pressed_b) {
            counter++;
        }
        
        osDelay(10);
    }
}
```

## 时间基准选择

| 函数 | 适用场景 |
|------|---------|
| `xTaskGetTickCount()` | RTOS 任务中调用（**推荐**） |
| `HAL_GetTick()` | 非 RTOS 上下文 / 调度器启动前 |

**推荐**: `key_scan_v(xTaskGetTickCount())` — 和 RTOS 任务上下文一致。

## 调用路径

```
Task_LX 或其他 RTOS 任务 (10ms 周期)
  ↓
key_scan_v(xTaskGetTickCount())  ← 驱动状态机
  ↓
s_key_read_pin_b()  ← 读 GPIO
  ↓
HAL_GPIO_ReadPin()  ← HAL 层
```

```
key_scan_v() 检测到边沿
  ↓
s_event_cb(id, event)  ← 触发回调（如果有注册）
```

```
业务代码
  ↓
key_state_copy_v(keys_st, KEY_NUM_em)  ← 获取快照
  ↓
检查 just_pressed_b / pressed_b  ← 响应按键
```

## 与旧架构对比

| 旧架构 (Key.c) | Linux 风格 (Drv_Key.c) |
|---------------|----------------------|
| `switch(i)` 映射枚举到 struct 成员 | 数组下标 = 枚举值，零分支 |
| `Key_Test()` 每键一段 copy-paste | 统一 for 循环，数据驱动 |
| `Key_Info_t` 暴露硬件+状态 | 硬件描述 (const) / 运行时状态 (不透明) 分离 |
| 手动调用 `Key_Init()` | `DRIVER_INIT` 自动初始化 |
| `GetKeyStatus()` 返回大 struct | `key_state_copy_v()` Copy 快照模式 |
| `RegisterKeyCallback(index, cb)` 每键一个回调 | `key_event_register_v(cb)` 统一回调 + 参数传递 key_id |
| `Key_Status_t` 只有电平 | `Key_State_t` 有电平 + 边沿（just_pressed/released） |

## 注意事项

1. **10ms 周期**: `key_scan_v()` 建议 10ms 调用一次。太快浪费 CPU，太慢影响消抖精度
2. **消抖时间**: `KEY_DEBOUNCE_MS = 20`，可根据硬件调整（机械按键通常 10~50ms）
3. **时间戳溢出**: `now_ms_ul - edge_tick_ms_ul` 使用 uint32_t 减法，自然处理 ~49 天溢出
4. **回调上下文**: 回调在 `key_scan_v()` 任务上下文中执行，**不要阻塞**
5. **UP 键低有效**: PA5 是低电平有效，硬件上可能是按下接地
6. **DRIVER_INIT**: `s_key_init_v()` 自动注册，无需手动调用

## 文件清单

| 文件 | 职责 |
|------|------|
| `Drv_Key.h` | 公开 API + 值类型（枚举、Key_State_t） |
| `Drv_Key.c` | 硬件描述表 + 运行时状态 + 消抖状态机 |
| `Key.c/h` (旧) | **已废弃**，旧架构实现 |
