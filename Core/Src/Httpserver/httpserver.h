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
#define HTTPD_200      "200 OK"
#define HTTPD_304      "304 Not Modified"

void http_server_init(void);
void erase_sectors();
void boot_jump();
int http_get_content_length(char* buf);
int flash_data(char* buf, int len);

#endif /* __HTTPSERVER_NETCONN_H__ */
