#ifndef __VALUE_TO_STR_H
#define __VALUE_TO_STR_H

#include "stdio.h"
#include <stdint.h>

int float_to_str(char *buf, size_t buf_size, float value, uint8_t decimal_places);
int int32_to_str(char *buf, size_t buf_size, int32_t value);

#endif
