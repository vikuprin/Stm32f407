#ifndef _MQTT_CLIENT_H
#define _MQTT_CLIENT_H

#if DEBUG_PRINT == 1
#define DEBUG_MQTT(...) printf("MQTT: "__VA_ARGS__);
#else
#define DEBUG_MQTT(...)
#endif

#include "main.h"
#include "lwip/apps/mqtt.h"
#include "lwip/apps/mqtt_priv.h"

void init_mqtt();
void start_mqtt();

void MqttClientTask(void *argument);

#endif
