#ifndef __FAN_H__
#define __FAN_H__
#include "main.h"

#if DEBUG_PRINT == 1
#define DEBUG_FAN(...) printf("FAN: "__VA_ARGS__);
#elif DEBUG_PRINT == 2
#define DEBUG_FAN(...) sprintf(msg, "FAN: "__VA_ARGS__);HAL_UART_Transmit_IT(&huart4, (uint8_t*)msg, strlen(msg));
#else
#define DEBUG_FAN(...)
#endif

uint8_t inflow_power1;

void set_inflow_fan1(uint8_t value);

#endif // __FAN_H__
