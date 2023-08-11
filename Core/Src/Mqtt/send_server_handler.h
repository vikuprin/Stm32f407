#ifndef __SEND_SERVER_HANDLER_H__
#define __SEND_SERVER_HANDLER_H__

#if DEBUG_PRINT == 1
#define DEBUG_SEND(...) printf("MQTT SEND: "__VA_ARGS__);
#else
#define DEBUG_SEND(...)
#endif

#include "main.h"

typedef enum
{
    CAPABILITIES = 0,
    SETTINGS,
    RELATION,
    FAULTS,
} send_device_e;

typedef enum
{
    ENTITY_CAPABILITIES = 0,
    ENTITY_SETTINGS,
} send_entity_e;

void send_server_task();
#endif
