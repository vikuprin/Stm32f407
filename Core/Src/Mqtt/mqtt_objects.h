#ifndef __MQTT_OBJECTS_H__
#define __MQTT_OBJECTS_H__

#include "main.h"

void get_str_capabiities(char *capabilities_str);
void get_str_entity_capabiities(char *capabilities_str, uint8_t entity_number);
void get_str_entity_relation(char *relation_str, uint8_t entity_number);
void get_str_entity_create(char *create_str, uint8_t entity_number);
void get_str_entity_settings(char *settings, uint8_t entity_number);
void get_str_settings(char *settings);
void get_str_errors(char *errors);
void get_str_system_status(char *system_str);
void get_str_temp_log(char *temp_log_str);
void get_str_extra_options(char *extra);

#endif //__MQTT_OBJECTS_H__
