#ifndef __SEND_SERVER_HANDLER_H__
#define __SEND_SERVER_HANDLER_H__

#include "main.h"

#if DEBUG_PRINT == 1
#define DEBUG_SEND(...) printf("MQTT SEND: "__VA_ARGS__);
#elif DEBUG_PRINT == 2
#define DEBUG_SEND(...) sprintf(msg, "MQTT SEND: "__VA_ARGS__);HAL_UART_Transmit_IT(&huart4, (uint8_t*)msg, strlen(msg));
#else
#define DEBUG_SEND(...)
#endif


typedef enum
{
    CAPABILITIES = 0,
    FAULTS,
} send_device_e;

void send_server();

#endif
