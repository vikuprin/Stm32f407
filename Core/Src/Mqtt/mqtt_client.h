#ifndef _MQTT_CLIENT_H
#define _MQTT_CLIENT_H

#if DEBUG_PRINT == 1
#define DEBUG_MQTT(...) printf("MQTT: "__VA_ARGS__);
#elif DEBUG_PRINT == 2
#define DEBUG_MQTT(...) sprintf(msg, "MQTT: "__VA_ARGS__);HAL_UART_Transmit_IT(&huart4, (uint8_t*)msg, strlen(msg));
#else
#define DEBUG_MQTT(...)
#endif

#include "main.h"
#include "lwip/apps/mqtt.h"
#include "lwip/apps/mqtt_priv.h"

char log_pub_topic[80];
char temp_log_pub_topic[80];
char mode_pub_topic[80];
char mode_topic[80];
char system_pub_topic[80];
char system_topic[80];
char workmode_topic[80];
char speed_topic[80];
char temp_limit_topic[80];
char state_topic[80];

void system_topic_handler(char *data);
void mode_topic_handler(char *data);
void master_topic_handler(char *data);
void publish_message(char topic[], char message[]);
void publish_message_qos0(char topic[], char message[]);
void set_mqtt_parameters();
void start_mqtt();
void init_mqtt();
void mqtt_sub_request_cb(void *arg, err_t result);
void mqtt_unsub_request_cb(void *arg, err_t result);

#endif
