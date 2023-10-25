#include "mqtt_message.h"
#include "mqtt_client.h"
#include "mqtt_objects.h"
#include <stdlib.h>
#include "cmsis_os.h"

void publish_auth(char *version, char *mac_address, char *series, char *subtype, char *xtal_freq)
{
    char *message = malloc(256);
    while (message == NULL)
    {
    	osDelay(10);
        message = malloc(256);
    }
    sprintf(message, "{\"type\": \"auth\", \"auth\":{\"device_mac\":\"%s\",\"version\":\"%s\"}, \"device_subtype\":{\"series\":\"%s\",\"subtype\":\"%s\",\"xtal_freq\":\"%s\"}}", mac_address, version, series, subtype, xtal_freq);
    DEBUG_MQTT("AUTH\n");
    DEBUG_MQTT("%s\n", message);
    DEBUG_MQTT("system_pub_topic %s\n", system_pub_topic);
    publish_message(system_pub_topic, message);
    free(message);
}


char *on_off[2] = {"off", "on"};

void publish_capabilities()
{
    char *capabilities = malloc(300);
    while (capabilities == NULL)
    {
        vTaskDelay(10);
        capabilities = malloc(300);
    }
    get_str_capabiities(capabilities);
    publish_message(mode_pub_topic, capabilities);
    free(capabilities);
}

void publish_temp_log()
{
	if(mqtt_status)
	{
		char *temp_log = malloc(300);
		while (temp_log == NULL)
		{
			vTaskDelay(10);
			temp_log = malloc(300);
		}
		get_str_temp_log(temp_log);
		publish_message(temp_log_pub_topic, temp_log);
		free(temp_log);
	}
}

void publish_system()
{
    char *system = malloc(300);
    while (system == NULL)
    {
        vTaskDelay(10);
        system = malloc(300);
    }
    get_str_system_status(system);
    DEBUG_MQTT("Device system %s\n", system);
    publish_message(log_pub_topic, system);
    free(system);
}

void publish_errors()
{
    char *errors = malloc(300);
    while (errors == NULL)
    {
        vTaskDelay(10);
        errors = malloc(300);
    }
    get_str_errors(errors);
    publish_message(system_pub_topic, errors);
    free(errors);
}

void publish_log(char *log)
{
	char *message = malloc(256);
    while (message == NULL)
    {
        vTaskDelay(10);
        message = malloc(256);
    }
    sprintf(message, "%s", log);
    DEBUG_MQTT("LOG %s\n", log);
    publish_message_qos0(log_pub_topic, message);
}

void publish_extra_options()
{

    char *extra = malloc(256);
    while (extra == NULL)
    {
        vTaskDelay(10);
        extra = malloc(256);
    }
    get_str_extra_options(extra);
    publish_message(log_pub_topic, extra);
    free(extra);
}

void publish_firmware_state(char *state)
{
    char *message = malloc(256);
    while (message == NULL)
    {
        vTaskDelay(10);
        message = malloc(256);
    }
    sprintf(message, "{\"firmware_state\":\"%s\"}", state);
    publish_message(log_pub_topic, message);
}
