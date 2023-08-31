#ifndef __RC_H__
#define __RC_H__
#include "main.h"

#if DEBUG_PRINT == 1
#define DEBUG_RC(...) printf("REMOTE CONTROL: "__VA_ARGS__);
#elif DEBUG_PRINT == 2
#define DEBUG_RC(...) sprintf(msg, "REMOTE CONTROL: "__VA_ARGS__);HAL_UART_Transmit_IT(&huart4, (uint8_t*)msg, strlen(msg));
#else
#define DEBUG_RC(...)
#endif


// PULT HOLDING
#define REG_SWITCH 0
#define REG_FAN 1
#define REG_TEMP 2
#define REG_WEEK 3
#define REG_HOUR 4
#define REG_MINUTE 5
#define REG_CHILD 6
#define REG_WORK 7
#define REG_WATER 8
#define REG_HOTWATER 9
#define REG_HEATER 10
#define REG_WARNING 11
#define REG_FLOW 12
#define REG_CLOCK 13

void check_remote_control(void);

#endif
