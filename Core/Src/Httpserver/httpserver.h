#ifndef __HTTPSERVER_NETCONN_H__
#define __HTTPSERVER_NETCONN_H__

#if DEBUG_PRINT == 1
#define DEBUG_SERVER(...) printf("SERVER: "__VA_ARGS__);
#elif DEBUG_PRINT == 2
#define DEBUG_SERVER(...) sprintf(msg, "SERVER: "__VA_ARGS__);HAL_UART_Transmit_IT(&huart4, (uint8_t*)msg, strlen(msg));
#else
#define DEBUG_SERVER(...)
#endif

#include "lwip/api.h"
void http_server_init(void);

#endif /* __HTTPSERVER_NETCONN_H__ */
