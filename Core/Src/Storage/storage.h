#ifndef _STORAGE_H
#define _STORAGE_H

#include "main.h"
#include "usart.h"

#if DEBUG_PRINT == 1
#define DEBUG_STORAGE(...) printf("STORAGE: "__VA_ARGS__);
#elif DEBUG_PRINT == 2
#define DEBUG_STORAGE(...) sprintf(msg, "STORAGE: "__VA_ARGS__);HAL_UART_Transmit_IT(&huart4, (uint8_t*)msg, strlen(msg));
#else
#define DEBUG_STORAGE(...)
#endif

#define CHECK_VALUE_1_0_0  1

void write_device_params();
void read_device_params();
void write_wireless_params();
void read_wireless_params();

void init_storage();
void reset_nvs_parameteres();

#endif
