#include "string.h"
#include "mqtt_client.h"
#include "mqtt_message.h"
#include "../../Inc/cJSON/cJSON.h"
#include "../Storage/storage.h"
#include "../../Inc/cJSON/utils.h"

void set_device_speed_arr(cJSON *data_json)
{
    uint8_t speed_num;
    check_js_param_u8(data_json, "speed_num", &speed_num);
    check_js_param_u8(data_json, "value", &device->speed_arr[speed_num]);
}

void set_sensor_value(cJSON *data_json)
{

    if (check_js_param_double(data_json, "out", &sensors_data->out))
        sensors_data->out_state = true;
    if (check_js_param_double(data_json, "in", &sensors_data->in))
        sensors_data->in_state = true;
    DEBUG_MQTT("Set out temp %0.1f\n", sensors_data->out);
    DEBUG_MQTT("Set in temp %0.1f\n", sensors_data->in);
}

void set_error(cJSON *data_json)
{
    cJSON *error = cJSON_GetObjectItem(data_json, "error_type");
    cJSON *state = cJSON_GetObjectItem(data_json, "state");
    if (error != NULL && state != NULL)
        DEBUG_MQTT("Set error %s state %i\n", error->valuestring, state->valueint);
}

void firmware_handler(cJSON *firmware_js)
{
//    if (check_js_param_char(firmware_js, "domain", &wireless_params->domain))
//    {
//        set_ota_url(wireless_params->domain);
//        write_wireless_params();
//    }
//    bool start_firmware = false;
//    if (check_js_param_u8(firmware_js, "start", &start_firmware))
//    {
//        if (start_firmware)
//            start_update_firmware_isr();
//    }
//    cJSON *firmware_option_js = cJSON_GetObjectItem(firmware_js, "firmware_option");
//    if (firmware_option_js != NULL)
//    {
//        char domain[100];
//        char version[15];
//        check_js_param_char(firmware_option_js, "domain", &domain);
//        set_ota_url(domain);
//        if (check_js_param_char(firmware_option_js, "version", &version))
//        {
//            if (strcmp(VERSION, version) != 0)
//                start_update_firmware_isr();
//        }
//        else
//            start_update_firmware_isr();
//    }
}

void error_handler(cJSON *error_js)
{
    if (check_js_param_u8(error_js, "temp_hot", &device->error_temp_hot))
    {
    }
    if (check_js_param_u8(error_js, "stop_hot", &device->error_stop_hot))
    {
    }
    if (check_js_param_u8(error_js, "temp_cold", &device->error_temp_cold))
    {
    }
    if (check_js_param_u8(error_js, "stop_cold", &device->error_stop_cold))
    {
    }
    if (cJSON_GetObjectItem(error_js, "reset_error") != NULL)
    {
        device->error_temp_hot = false;
        device->error_stop_hot = false;
        device->error_temp_cold = false;
        device->error_stop_cold = false;
    }
}
void reset_handler(cJSON *reset_js)
{
    char reset_option[50];
    if (check_js_param_char(reset_js, "wireless", &reset_option))
    {
        first_init_wireless();
    }
    if (check_js_param_char(reset_js, "device", &reset_option))
    {
        first_init_device();
    }
    if (check_js_param_char(reset_js, "all", &reset_option))
    {
        first_init_device();
        first_init_wireless();
    }
}

void logging_handler(cJSON *logging_js)
{
    char log_type[50];
    check_js_param_char(logging_js, "type", log_type);
    if (strcmp("system", log_type) == 0)
    {
        publish_system();
    }
    else if (strcmp("capabilities", log_type) == 0)
    {
        publish_capabilities();
    }
    else if (strcmp("extra_options", log_type) == 0)
    {
        publish_extra_options();
    }
}

void set_extra_options_handler(cJSON *data_json)
{
    check_js_param_u8(data_json, "deviation", &device->extra_options.deviation);
    check_js_param_u8(data_json, "check_time", &device->extra_options.check_time);
    check_js_param_u8(data_json, "step_pwm", &device->extra_options.step_pwm);

    check_js_param_int(data_json, "Kp", &device->extra_options.Kp);
    check_js_param_u8(data_json, "Ki", &device->extra_options.Ki);
    check_js_param_u8(data_json, "Kd", &device->extra_options.Kd);

    check_js_param_u8(data_json, "ten_state", &device->ten_state);

    DEBUG_MQTT("Set deviation %i\n", device->extra_options.deviation);
    DEBUG_MQTT("Set check_time %i\n", device->extra_options.check_time);
    DEBUG_MQTT("Set step_pwm %i\n", device->extra_options.step_pwm);
    DEBUG_MQTT("Set Kp %i\n", device->extra_options.Kp);
    DEBUG_MQTT("Set Ki %i\n", device->extra_options.Ki);
    DEBUG_MQTT("Set Kd %i\n", device->extra_options.Kd);
    DEBUG_MQTT("ten_state %i\n", device->ten_state);
    write_device_params();
}

void set_master_data_js(cJSON *data_json)
{
    char *master_topic = malloc(100);
    while (master_topic == NULL)
    {
        vTaskDelay(10);
        master_topic = malloc(100);
    }
    check_js_param_char(data_json, "master_topic", &master_topic);

    if (strcmp(device->smart.master_topic_system, master_topic) == 0)
    {
    	DEBUG_MQTT("SET ONLINE STATE\n");
        check_js_param_int(data_json, device->smart.parameter, &device->smart.value);
    }
    free(master_topic);
}

void system_topic_handler(char *data)
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
    // Ищем основные команды
    cJSON *error_js_arr = cJSON_GetObjectItem(data_json, "errors");
    cJSON *reset_js_arr = cJSON_GetObjectItem(data_json, "reset");
    cJSON *firmware_js_arr = cJSON_GetObjectItem(data_json, "firmware");
    cJSON *test_sensor_js = cJSON_GetObjectItem(data_json, "test_sensor");
    cJSON *loggin_js = cJSON_GetObjectItem(data_json, "logging");
    cJSON *extra_options_js = cJSON_GetObjectItem(data_json, "extra_options");
    cJSON *master_data_js = cJSON_GetObjectItem(data_json, "master_data");
    if (master_data_js != NULL)
        set_master_data_js(master_data_js);
    if (loggin_js != NULL)
        logging_handler(loggin_js);
    if (test_sensor_js != NULL)
        set_sensor_value(test_sensor_js);
    if (extra_options_js != NULL)
        set_extra_options_handler(extra_options_js);
    if (error_js_arr != NULL)
    {
        int js_size = cJSON_GetArraySize(error_js_arr);

        DEBUG_MQTT("Get error_js_arr %i\n", js_size);
        for (int count = 0; count < js_size; count++)
        {
            cJSON *error_js = cJSON_GetArrayItem(error_js_arr, count);
            error_handler(error_js);
        }
    }
    if (reset_js_arr != NULL)
    {
        int js_size = cJSON_GetArraySize(reset_js_arr);

        DEBUG_MQTT("Get reset_js_arr %i\n", js_size);
        for (int count = 0; count < js_size; count++)
        {
            cJSON *reset_js = cJSON_GetArrayItem(reset_js_arr, count);
            reset_handler(reset_js);
        }
    }
    if (firmware_js_arr != NULL)
    {
        int js_size = cJSON_GetArraySize(firmware_js_arr);

        DEBUG_MQTT("Get firmware_js_arr %i\n", js_size);
        for (int count = 0; count < js_size; count++)
        {
            cJSON *firmware_js = cJSON_GetArrayItem(firmware_js_arr, count);
            firmware_handler(firmware_js);
        }
    }
    if (extra_options_js != NULL)
    {
        set_extra_options_handler(extra_options_js);
    }
    cJSON_Delete(data_json);
}
