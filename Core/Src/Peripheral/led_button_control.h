#ifndef __LED_BUTTON_CONTROL_H__
#define __LED_BUTTON_CONTROL_H__
#include "main.h"

#if DEBUG_PRINT == 1
#define DEBUG_LED_BUT_CONTROL(...) printf("DEBUG_LED_BUT_CONTROL: "__VA_ARGS__);
#elif DEBUG_PRINT == 2
#define DEBUG_LED_BUT_CONTROL(...) sprintf(msg, "DEBUG_LED_BUT_CONTROL: "__VA_ARGS__);HAL_UART_Transmit_IT(&huart4, (uint8_t*)msg, strlen(msg));
#else
#define DEBUG_LED_BUT_CONTROL(...)
#endif

void link_callback_IP();
void button_handler();

#endif // __LED_BUTTON_CONTROL_H__

