#ifndef __DAMPER_H__
#define __DAMPER_H__

#include "main.h"

#if DEBUG_PRINT == 1
#define DEBUG_DAMP(...) printf("DAMPER: "__VA_ARGS__);
#elif DEBUG_PRINT == 2
#define DEBUG_DAMP(...) sprintf(msg, "DAMPER: "__VA_ARGS__);HAL_UART_Transmit_IT(&huart4, (uint8_t*)msg, strlen(msg));
#else
#define DEBUG_DAMP(...)
#endif

uint8_t damper_state; //0-закрыт, 1-открывается, 2-открыт

void close_damper();
void open_damper();
void DamperTask(void *argument);

#endif
