#ifndef __TEN_H__
#define __TEN_H__
#include "main.h"

#if DEBUG_PRINT == 1
#define DEBUG_TEN(...) printf("TEN: "__VA_ARGS__);
#elif DEBUG_PRINT == 2
#define DEBUG_TEN(...) sprintf(msg, "TEN: "__VA_ARGS__);HAL_UART_Transmit_IT(&huart4, (uint8_t*)msg, strlen(msg));
#else
#define DEBUG_TEN(...)
#endif

uint8_t ten_power;

void ten_handler();
void set_ten_power(uint16_t value);

#endif // __TEN_H__
