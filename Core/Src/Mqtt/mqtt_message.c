#include "mqtt_message.h"
#include "mqtt_client.h"
#include "mqtt_objects.h"
#include <stdlib.h>

void publish_auth(char *version, char *mac_address, char *series, char *subtype, char *xtal_freq)
{
    char message[255];
    sprintf(message, "{\"type\": \"auth\", \"auth\":{\"device_mac\":\"%s\",\"version\":\"%s\"}, \"device_subtype\":{\"series\":\"%s\",\"subtype\":\"%s\",\"xtal_freq\":\"%s\"}}", mac_address, version, series, subtype, xtal_freq);
    DEBUG_MQTT("AUTH\n");
    DEBUG_MQTT("%s\n", message);
    DEBUG_MQTT(" system_pub_topic %s\n", system_pub_topic);
    publish_message(system_pub_topic, message);
}

char *on_off[2] = {"off", "on"};

void publish_capabilities()
{
    char *capabilities = malloc(200);
    get_str_capabiities(capabilities);
    publish_message(mode_pub_topic, capabilities);
    free(capabilities);
}

void publish_temp_log()
{
    char *temp_log = malloc(800);
    get_str_temp_log(temp_log);
    publish_message(temp_log_pub_topic, temp_log);
    free(temp_log);
}

void publish_system()
{
    char *system = malloc(300);
    get_str_system_status(system);
    DEBUG_MQTT("Device system %s\n", system);
    publish_message(log_pub_topic, system);
    free(system);
}

void publish_settings()
{
    char *settings = malloc(600);
    get_str_settings(settings);
    publish_message(mode_pub_topic, settings);
    free(settings);
}

void publish_errors()
{
    char *errors = malloc(600);
    get_str_errors(errors);
    publish_message(system_pub_topic, errors);
    free(errors);
}

void publish_log(char *log)
{

    char message[255];
    sprintf(message, "%s", log);
    DEBUG_MQTT("LOG %s\n", log);
    publish_message_qos0(log_pub_topic, message);
}

void publish_extra_options()
{

    char *extra = malloc(600);
    get_str_extra_options(extra);
    publish_message(log_pub_topic, extra);
    free(extra);
}

void publish_firmware_state(char *state)
{
    char message[255];
    sprintf(message, "{\"firmware_state\":\"%s\"}", state);
    publish_message(log_pub_topic, message);
}
