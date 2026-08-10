# Menu — 树形菜单系统（Linux menuconfig 风格）

## 概述

OLED 菜单系统，采用 Linux `menuconfig` 风格的树形结构。**框架负责导航和显示，用户只负责声明树结构和写叶子 draw 函数。**

- **显示**: 128×64 OLED (I2C 非阻塞)
- **输入**: 4 按键 (KEY1~KEY4) → keyfunc subscriber 机制
- **架构**: `menu_base_t` 基类 + `menu_node_t` 派生类，`container_of` 下转型
- **刷新**: 事件驱动 (dirty flag)，非轮询
- **初始化**: `DRIVER_INIT(menu_oled_init_v)` 自动注册

## 架构设计

### 两层 struct 继承 — 与 Linux `struct device` 一致

```c
struct menu_base_t {              /* 基类 — 对外暴露的最小接口 */
    const char *name;
};

struct menu_node_t {              /* 派生类 — 框架内部完整实现 */
    struct menu_base_t base;      /* 基类嵌入 (必须是第一个成员) */
    enum menu_kind_e kind;
    void (*draw)(struct menu_node_t *self);
    struct menu_node_t *parent, *first_child, *next;
    uint8_t cursor, view_offset;
    int32_t *data;  float *data_f;
    /* ... 更多字段 ... */
};
```

与 Linux 内核的对应关系：

| Linux 内核 | 本菜单系统 | 说明 |
|-----------|-----------|------|
| `struct device` | `struct menu_base_t` | 对外暴露的基类 |
| `struct gpio_device` | `struct menu_node_t` | 内部完整实现 |
| `to_gpio_device(dev)` | `to_menu_node(base)` | `container_of` 下转型 |
| `device_create()` 返回 `struct device *` | `menu_create_item()` 返回 `struct menu_base_t *` | 工厂函数返回基类指针 |

**关键原则**：外部模块（如电池电压刷新）只持有 `struct menu_base_t *`，无法直接访问 `menu_node_t` 内部字段。需要操作完整节点时通过 `to_menu_node()` 下转型。

### container_of 下转型

```c
/* menu.h 中定义 */
#define to_menu_node(base_ptr) \
    container_of((base_ptr), struct menu_node_t, base)

/* 使用示例: menu_request_refresh 接收 base 指针, 内部下转型 */
void menu_request_refresh(struct menu_base_t *base)
{
    struct menu_node_t *me = to_menu_node(base);
    if (base && s_current_pst == me)
        s_dirty_b = true;
}
```

`container_of` 宏由 `Key_func.h` 提供（通过 `menu.h → Key_func.h` 包含链可用）。

## 节点类型

| 类型 | 特征 | 显示方式 |
|------|------|---------|
| `MENU_FOLDER` | 有子节点 (`first_child != NULL`) | 框架自动显示子项列表 + 光标 `>>` |
| `MENU_LEAF` | 无子节点 (`first_child == NULL`) | 调用 `draw()` 自定义显示 |

文件夹通用显示布局 (64px 高度):
```
(0, 0)   标题 (8×16 字体, 16px)
(0, 20)  子项 view_offset+0  (6×8 字体, 14px 行距)
(0, 34)  子项 view_offset+1
(0, 48)  子项 view_offset+2
```
子项类型标记: `[+]` 文件夹, `[#]` 可编辑叶子, ` > ` 普通叶子。
超过 `MENU_VISIBLE_ROWS` (3) 项时自动滚动, 右侧显示 `^`/`v` 指示器。

## 文件清单

| 文件 | 职责 |
|------|------|
| `menu.h` | 基类/派生类定义, 构建宏, 框架 API |
| `menu.c` | 核心实现: 导航, 编辑模式, 滚动, 显示 |
| `OLED_Menu.h` | 根节点 extern + 外部可见的 base 指针 |
| `OLED_Menu.c` | 用户侧: 菜单树构建 + 叶子 draw 函数 |

## 构建 API — Create_Menu_* 宏

**约定**: `me` 传结构体本身 (不是指针), 宏内部用 `&(me)` 取地址传给函数, 用 `(me).field` 设置字段。

```c
/* 文件夹 */
Create_Menu_Folder(parent, me, name)

/* 普通叶子 (只读显示) */
Create_Menu_Leaf(parent, me, name, draw_fn)

/* int32 可编辑叶子 */
Create_Menu_Leaf_Int(parent, me, name, draw_fn, data_ptr)

/* int32 可编辑 + 范围限制 */
Create_Menu_Leaf_Range(parent, me, name, draw_fn, data_ptr, min, max)

/* float 可编辑叶子 */
Create_Menu_Leaf_Float(parent, me, name, draw_fn, data_ptr)

/* float 可编辑 + 范围限制 */
Create_Menu_Leaf_Float_Range(parent, me, name, draw_fn, data_ptr, min, max)
```

`menu_create_item()` 返回 `struct menu_base_t *` — 外部模块可保存此返回值用于 `menu_request_refresh()`。

## 按键映射与操作

| 按键 | 功能 (浏览模式) | 功能 (编辑模式) |
|------|---------------|---------------|
| KEY4 | UP — 光标上移 | UP — 增加值 |
| KEY2 | DOWN — 光标下移 | DOWN — 减小值 |
| KEY3 单击 | ENTER — 进入子项 / 进入编辑 | 退出编辑 |
| KEY3 双击 | — | 循环切换步进值 |
| KEY3 长按 | — | 恢复默认值 |
| KEY1 | BACK — 退出编辑 / 返回上级 | BACK — 退出编辑 |

步进值表: `{0.1, 1, 10, 100}` — 双击 ENTER 在这 4 个值之间循环。

## 编辑模式

```
叶子页面 → 单击 ENTER → 进入编辑模式
  → UP/DOWN 修改 *data 或 *data_f
  → 双击 ENTER 切换步进 (s:1 → s:10 → s:100 → s:0.1)
  → 长按 ENTER 恢复默认值 (menu_set_default_int/float 预设)
  → 单击 BACK 退出编辑 (不导航)
```

屏幕底部显示: 当前值 + 步进 (`s:1`) + 模式 (`EDIT` / `[ENT]`)。

## 外部数据源实时刷新

```c
/* OLED_Menu.c — 创建菜单时保存 base 指针 */
struct menu_base_t *g_battery_oled_pst;
g_battery_oled_pst = Create_Menu_Leaf(&g_menu_select, g_battery_st,
                                       "battery", s_draw_battery_v);

/* 外部模块 — 数据更新后请求刷新 */
void v_Bat_Voltage_Data_Handle(void)
{
    /* ... 计算电压电流 ... */
    menu_request_refresh(g_battery_oled_pst);  /* 只在电池页面显示时触发重绘 */
}
```

**隔离原理**: `menu_request_refresh(base)` 内部检查 `s_current_pst == to_menu_node(base)`，只有当前页面匹配时才设置 dirty flag。不同数据源传不同 base 指针，互不干扰。

## 导航模型

```
s_current_pst = 当前视图节点
  有子节点 → s_menu_folder_draw() 自动显示子项列表, cursor 控制高亮
  无子节点 → draw() 自定义显示

UP/DOWN → cursor ± 1 (循环), 自动更新 view_offset (滚动)
ENTER   → 进入第 cursor 个子项 (压入导航栈)
BACK    → 弹出导航栈, 恢复父节点 cursor

s_nav_stack[8] — 保存导航路径, BACK 时逐层返回
```

## 使用示例

```c
/* 1. 声明节点 */
struct menu_node_t g_root;
static struct menu_node_t g_folder_pid;
static struct menu_node_t g_item_kp;
static int32_t s_pid_kp = 100;

/* 2. 构建树 */
Create_Menu_Folder(NULL, g_root, "Main");
Create_Menu_Folder(&g_root, g_folder_pid, "PID");
Create_Menu_Leaf_Range(&g_folder_pid, g_item_kp, "Kp", NULL,
                       &s_pid_kp, 0, 1000);
menu_set_default_int(&g_item_kp, 100);

/* 3. 自动初始化 */
void menu_oled_init_v(void) {
    menu_init_v();
    s_build_menu_tree();
}
DRIVER_INIT(menu_oled_init_v);
```

## 注意事项

1. **`me` 传结构体, 不传指针** — 所有 `Create_Menu_*` 宏的第二个参数传结构体本身 (如 `g_root`), 不是 `&g_root`。`parent` 参数仍然传指针。
2. **`draw()` 函数通过 `self->base.name` 访问名称** — 不要直接访问 `self->name`。
3. **`menu_request_refresh()` 接收 `struct menu_base_t *`** — 保存 `Create_Menu_*` 的返回值，外部模块通过此指针触发刷新。
4. **无轮询** — 所有刷新由事件驱动: 按键 (dirty flag) 或外部数据 (`menu_request_refresh`)。
5. **`s_dirty_b` 是全局的** — 同一时刻只有一个页面可以触发重绘，但 `menu_request_refresh` 的 node 匹配保证了只有正确的页面会响应。
6. **OLED_Menu.c 的 draw 函数中不要有 `delay` 或阻塞操作** — draw 在 `menu_task_v()` 上下文中执行，阻塞会影响整个控制循环。
