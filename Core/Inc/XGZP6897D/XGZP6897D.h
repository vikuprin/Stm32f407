#ifndef __XGZP6897D_H
#define __XGZP6897D_H

#include "main.h"

#if DEBUG_PRINT == 1
#define DEBUG_XGZ(...) printf("XGZ: "__VA_ARGS__);
#elif DEBUG_PRINT == 2
#define DEBUG_XGZ(...) sprintf(msg, "XGZ: "__VA_ARGS__);HAL_UART_Transmit_IT(&huart4, (uint8_t*)msg, strlen(msg));
#else
#define DEBUG_XGZ(...)
#endif

#define	SLAVE_ADDR     0x6D<<1
#define TIMEOUT        0xFFFF

void get_xgz_data();

#endif




