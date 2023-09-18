#ifndef __MODES_H__
#define __MODES_H__

#include "main.h"

#if DEBUG_PRINT == 1
#define DEBUG_MODES(...) printf("MODES: "__VA_ARGS__);
#elif DEBUG_PRINT == 2
#define DEBUG_MODES(...) sprintf(msg, "MODES: "__VA_ARGS__);HAL_UART_Transmit_IT(&huart4, (uint8_t*)msg, strlen(msg));
#else
#define DEBUG_MODES(...)
#endif

uint8_t cold_speed;
uint8_t fan_count;

void MainTask(void *argument);

#endif // __MODES_H__
