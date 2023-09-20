#include "mqtt_client.h"
#include "../../Inc/cJSON/cJSON.h"
#include "../Storage/storage.h"
#include "../../Inc/cJSON/utils.h"

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
	check_js_param_int(data_json, "temp_limit", &device->temp_limit);
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
    if (data == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        return;
    }
    cJSON *capabilities_js_arr = cJSON_GetObjectItem(data_json, "capabilities");
    cJSON *settings_js_arr = cJSON_GetObjectItem(data_json, "settings");

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
    cJSON_Delete(data_json);
    write_device_params();
}
