#ifndef __TEN_H__
#define __TEN_H__
#include "main.h"

#if DEBUG_PRINT == 1
#define DEBUG_TEN(...) printf("TEN: "__VA_ARGS__);
#else
#define DEBUG_TEN(...)
#endif

uint8_t ten_power;

void set_ten_power(uint8_t value);

#endif // __TEN_H__
