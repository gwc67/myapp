#include "value_to_str.h"
#include "stdio.h"
#include <stdint.h>

int float_to_str(char *buf, size_t buf_size, float value, uint8_t decimal_places)
{
    if (buf == NULL || buf_size < 2) {
        return -1;
    }

    /* 限制小数位数 */
    if (decimal_places > 6) {
        decimal_places = 6;
    }

    size_t pos = 0;

    /* 处理负数 */
    if (value < 0.0f) {
        buf[pos++] = '-';
        value = -value;
    }

    /* 计算缩放因子 */
    size_t scale = 1;
    for (uint8_t i = 0; i < decimal_places; i++) {
        scale *= 10;
    }

    /* 四舍五入 */
    float rounded = value * (float)scale + 0.5f;
    size_t total = (size_t)rounded;

    size_t integer = total / scale;
    size_t fraction = total % scale;

    /* 写入整数部分 */
    char temp[16];
    int i = 0;

    if (integer == 0) {
        temp[i++] = '0';
    } else {
        while (integer > 0) {
            temp[i++] = '0' + (integer % 10);
            integer /= 10;
        }
    }

    /* 检查缓冲区 */
    if (pos + i >= buf_size) return -1;

    /* 反转写入 */
    for (int j = i - 1; j >= 0; j--) {
        buf[pos++] = temp[j];
    }

    /* 写入小数部分 */
    if (decimal_places > 0) {
        if (pos >= buf_size - 1) return -1;
        buf[pos++] = '.';

        /* 从高位到低位写入 */
        for (int j = decimal_places - 1; j >= 0; j--) {
            size_t divisor = 1;
            for (int k = 0; k < j; k++) divisor *= 10;

            size_t digit = fraction / divisor;
            fraction %= divisor;

            if (pos >= buf_size - 1) return -1;
            buf[pos++] = '0' + digit;
        }
    }

    buf[pos] = '\0';
    return (int)pos;
}


int int32_to_str(char *buf, size_t buf_size, int32_t value)
{
    if (buf == NULL || buf_size < 2) {
        return -1;
    }

    size_t pos = 0;

    /* 处理 INT32_MIN（-2147483648），避免取绝对值溢出 */
    if (value == INT32_MIN) {
        const char *min_str = "-2147483648";
        for (int i = 0; min_str[i] != '\0'; i++) {
            if (pos >= buf_size - 1) return -1;
            buf[pos++] = min_str[i];
        }
        buf[pos] = '\0';
        return (int)pos;
    }

    /* 处理负数 */
    if (value < 0) {
        buf[pos++] = '-';
        value = -value;
    }

    /* 提取各位数字（逆序） */
    char temp[12];
    int i = 0;

    if (value == 0) {
        temp[i++] = '0';
    } else {
        while (value > 0) {
            temp[i++] = '0' + (value % 10);
            value /= 10;
        }
    }

    /* 检查缓冲区 */
    if (pos + i >= buf_size) return -1;

    /* 反转写入 */
    for (int j = i - 1; j >= 0; j--) {
        buf[pos++] = temp[j];
    }

    buf[pos] = '\0';
    return (int)pos;
}