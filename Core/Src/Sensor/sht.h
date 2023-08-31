#ifndef __SHT_H__
#define __SHT_H__

#include "main.h"

#if DEBUG_PRINT == 1
#define DEBUG_SHT(...) printf("SHT30: "__VA_ARGS__);
#elif DEBUG_PRINT == 2
#define DEBUG_SHT(...) sprintf(msg, "SHT30: "__VA_ARGS__);HAL_UART_Transmit_IT(&huart4, (uint8_t*)msg, strlen(msg));
#else
#define DEBUG_SHT(...)
#endif

void get_sht_data();
void init_sht_devices();

#endif // __SENSOR_H__
