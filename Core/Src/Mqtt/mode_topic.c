#include "mqtt_client.h"
#include "../../Inc/cJSON/cJSON.h"
#include "../Storage/storage.h"
#include "../../Inc/cJSON/utils.h"
#include "cmsis_os.h"
#include "string.h"


#if DEBUG_PRINT == 1
#define DEBUG_MQTT_MODE(...) printf("MQTT_MODE: "__VA_ARGS__);
#elif DEBUG_PRINT == 2
#define DEBUG_MQTT_MODE(...) sprintf(msg, "MQTT_MODE: "__VA_ARGS__);HAL_UART_Transmit_IT(&huart4, (uint8_t*)msg, strlen(msg));
#else
#define DEBUG_MQTT_MODE(...)
#endif

extern mqtt_client_t *client;
void *arg;

void capabilities(cJSON *capabilities_js)
{
    check_char_on_off(capabilities_js, "on_off", &device->state);
    check_char_mode(capabilities_js, "mode", &device->mode);
    check_js_param_u8(capabilities_js, "speed", &device->inflow_speed);
    check_js_param_u8(capabilities_js, "heat", &device->temp_limit);
    device_check = *device;
}

void set_ten_settings(cJSON *data_json)
{
	check_js_param_int(data_json, "heat", &device->temp_limit);
    DEBUG_MQTT("Set ten temp_limit = %i", device->temp_limit);
    write_device_params();
}

void settings_handler(cJSON *settings_js)
{
    cJSON *ten_js = cJSON_GetObjectItem(settings_js, "ten");

    if (ten_js != NULL)
        set_ten_settings(ten_js);
    if (check_js_param_char(settings_js, "mqtt_ip", &wireless_params->vakio_mqtt.host))
    {
        write_wireless_params();
        set_mqtt_parameters();
    }
    if (check_js_param_u8(settings_js, "damper", &device->damper))
        write_device_params();
}

void relation(cJSON *relation_js)
{
	err_t err;
    cJSON *dependence_js = cJSON_GetObjectItem(relation_js, "dependence");
    uint8_t on_off_dependence;
    check_char_on_off(relation_js, "on_off_dependence", &on_off_dependence);
    if (dependence_js != NULL)
    {
        check_js_param_char(dependence_js, "parametr", &device->smart.parameter);
        check_js_param_int(dependence_js, "device_id_master", &device->smart.master_id);
        char* data_topic = malloc(50);
        while (data_topic == NULL)
        {
            vTaskDelay(10);
            data_topic = malloc(50);
        }
        data_topic[0] = 0;
        char* system_topic = malloc(50);
        while (system_topic == NULL)
        {
            vTaskDelay(10);
            system_topic = malloc(50);
        }
        system_topic[0] = 0;
        if (check_js_param_char(dependence_js, "data_topic", data_topic) && check_js_param_char(dependence_js, "system_topic", system_topic))
        {
            device->smart.set = true;
        }

        if (strcmp(device->smart.master_topic_data, data_topic) != 0)
        {
        	DEBUG_MQTT("New master_topic_data\n");
        	err = mqtt_unsubscribe(client, device->smart.master_topic_data, mqtt_unsub_request_cb, arg);
			if (err == ERR_OK)
				DEBUG_MQTT("unsubscribe to master_topic_data\n");
            strcpy(device->smart.master_topic_data, data_topic);
            err = mqtt_subscribe(client, device->smart.master_topic_data, 2, mqtt_sub_request_cb, arg);
			if (err == ERR_OK)
				DEBUG_MQTT("subscribe to master_topic_data\n");
        }
        if (strcmp(device->smart.master_topic_system, system_topic) != 0)
        {
        	DEBUG_MQTT("New master_topic_system\n");
        	err = mqtt_unsubscribe(client, device->smart.master_topic_system, mqtt_unsub_request_cb, arg);
			if (err == ERR_OK)
				DEBUG_MQTT("unsubscribe to master_topic_system\n");
            strcpy(device->smart.master_topic_system, system_topic);
            err = mqtt_subscribe(client, device->smart.master_topic_system, 2, mqtt_sub_request_cb, arg);
			if (err == ERR_OK)
				DEBUG_MQTT("subscribe to master_topic_system\n");
        }

        if (check_js_param_int(dependence_js, "min_value", &device->smart.limit))
        {
        	DEBUG_MQTT(" min_value %i\n", device->smart.limit);
        	DEBUG_MQTT(" max_speed %i\n", device->smart.max_speed);
        }
        device->smart.set = strlen(device->smart.master_topic_data) > 2;
        free(data_topic);
        free(system_topic);
    }
    if (on_off_dependence == 0)
    {
    	DEBUG_MQTT("OFF DEPENDENCE/n");
    	err = mqtt_unsubscribe(client, device->smart.master_topic_data, mqtt_unsub_request_cb, arg);
		if (err == ERR_OK)
			DEBUG_MQTT("unsubscribe to master_topic_data\n");
        device->smart.master_topic_data[0] = ' ';
    	err = mqtt_unsubscribe(client, device->smart.master_topic_system, mqtt_unsub_request_cb, arg);
		if (err == ERR_OK)
			DEBUG_MQTT("unsubscribe to master_topic_system\n");
        device->smart.master_topic_system[0] = ' ';
        device->smart.set = false;
    }
    device_check = *device;
}

void mode_topic_handler(char *data)
{
    if (data == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        return;
    }
    cJSON *data_json = cJSON_Parse(data);
    while (data_json == NULL)
    {
        vTaskDelay(10);
        data_json = cJSON_Parse(data);
    }
    cJSON *capabilities_js_arr = cJSON_GetObjectItem(data_json, "capabilities");
    cJSON *settings_js_arr = cJSON_GetObjectItem(data_json, "settings");
    cJSON *relation_js_arr = cJSON_GetObjectItem(data_json, "relation");

    if (settings_js_arr != NULL)
    {
        int js_size = cJSON_GetArraySize(settings_js_arr);
        DEBUG_MQTT("Get settings_js_arr %i\n", js_size);
        for (int count = 0; count < js_size; count++)
        {
            cJSON *settings_js = cJSON_GetArrayItem(settings_js_arr, count);
            settings_handler(settings_js);
        }
    }

    if (capabilities_js_arr != NULL)
    {
        int js_size = cJSON_GetArraySize(capabilities_js_arr);
        DEBUG_MQTT("Get capabilities %i\n", js_size);
        for (int count = 0; count < js_size; count++)
        {
            cJSON *capabilities_js = cJSON_GetArrayItem(capabilities_js_arr, count);
            capabilities(capabilities_js);
        }
    }

    if (relation_js_arr != NULL)
    {
        if (relation_js_arr->type == cJSON_Array)
        {
            // Получаем количество команд, которое прислали
            int js_size = cJSON_GetArraySize(relation_js_arr);
            DEBUG_MQTT("Get relation size %i\n", js_size);
            // Перебираем все команды и применяем
            for (int cap = 0; cap < js_size; cap++)
            {
                cJSON *relation_js = cJSON_GetArrayItem(relation_js_arr, cap);
                relation(relation_js);
            }
        }
        else
        {
            relation(relation_js_arr);
        }
    }

    cJSON_Delete(data_json);
    write_device_params();
}
