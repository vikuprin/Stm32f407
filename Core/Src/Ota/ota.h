#ifndef _OTA_H
#define _OTA_H

#include "main.h"
#include <arch.h>

#if DEBUG_PRINT == 1
#define DEBUG_OTA(...) printf("MQTT: "__VA_ARGS__);
#elif DEBUG_PRINT == 2
#define DEBUG_OTA(...) sprintf(msg, "MQTT: "__VA_ARGS__);HAL_UART_Transmit_IT(&huart4, (uint8_t*)msg, strlen(msg));
#else
#define DEBUG_OTA(...)
#endif

enum tcp_client_states
{
  ES_NONE = 0,
  ES_CONNECTED,
  ES_RECEIVING,
  ES_CLOSING
};

struct tcp_client_struct
{
  u8_t state;
  u8_t retries;
  struct tcp_pcb *pcb;
  struct pbuf *p;
};

#endif