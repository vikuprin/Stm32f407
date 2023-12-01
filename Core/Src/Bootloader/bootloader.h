#ifndef __BOOTLOADER_H__
#define __BOOTLOADER_H__

#include "main.h"

#if DEBUG_PRINT == 1
#define DEBUG_BOOT(...) printf("BOOTLOADER: "__VA_ARGS__);
#elif DEBUG_PRINT == 2
#define DEBUG_BOOT(...) sprintf(msg, "BOOTLOADER: "__VA_ARGS__);HAL_UART_Transmit_IT(&huart4, (uint8_t*)msg, strlen(msg));
#else
#define DEBUG_BOOT(...)
#endif

void bootloader();

#endif // __BOOTLOADER_H__
