#include "mqtt_client.h"
#include "string.h"

void master_topic_handler(char *data)
{
    device->smart.value = atoi(data);
    DEBUG_MQTT("get message master_topic_handler\n");
}
