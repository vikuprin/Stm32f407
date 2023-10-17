#include "mqtt_objects.h"
#include "string.h"
#include "../../Inc/cJSON/cJSON.h"
#include "cmsis_os.h"

char *on_off_[2] = {"off", "on"};
char *modes[4] = {"inflow", "inflow_max", "smart"};

void get_str_capabiities(char *capabilities_str)
{
    cJSON *root, *capabilities;
    root = cJSON_CreateObject();
    while (root == NULL)
    {
    	osDelay(10);
        root = cJSON_CreateObject();
    }
    capabilities = cJSON_CreateObject();
    while (capabilities == NULL)
    {
    	osDelay(10);
        capabilities = cJSON_CreateObject();
    }
    cJSON_AddItemToObject(root, "capabilities", capabilities);
    cJSON_AddStringToObject(capabilities, "mode", modes[device->mode]);
    cJSON_AddStringToObject(capabilities, "on_off", on_off_[device->state]);
    cJSON_AddNumberToObject(capabilities, "speed", device->inflow_speed);
    cJSON_AddNumberToObject(capabilities, "ten_state", device->ten_state);
    cJSON_AddNumberToObject(capabilities, "heat", device->temp_limit);
    char *js_str = cJSON_Print(root);
    while (js_str == NULL)
    {
    	osDelay(10);
        js_str = cJSON_Print(root);
    }
    strcpy(capabilities_str, js_str);
    cJSON_Delete(root);
    cJSON_free(js_str);
}

void get_str_errors(char *errors)
{
    cJSON *root, *errors_js;
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "errors", errors_js = cJSON_CreateObject());
    cJSON_AddNumberToObject(errors_js, "temp_hot", device->error_temp_hot);
    cJSON_AddNumberToObject(errors_js, "temp_cold", device->error_temp_cold);
    cJSON_AddNumberToObject(errors_js, "stop_hot", device->error_stop_hot);
    cJSON_AddNumberToObject(errors_js, "stop_cold", device->error_stop_cold);
    cJSON_AddNumberToObject(errors_js, "ds18_bus", device->error_ds18b20);

    char *js_str = cJSON_Print(root);
    strcpy(errors, js_str);
    cJSON_Delete(root);
    cJSON_free(js_str);
}

void get_str_system_status(char *system_str)
{
    cJSON *root;
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "ten_power", device->ten_power);
    cJSON_AddNumberToObject(root, "temp_out", sensors_data->out);
    cJSON_AddNumberToObject(root, "temp_in", sensors_data->in);
    cJSON_AddNumberToObject(root, "state_out_sensor", sensors_data->out_state);
    cJSON_AddNumberToObject(root, "state_in_sensor", sensors_data->in_state);

    // {"entity_on_off_dependence":"off","entity_dependence":{"device_id_master":0,"mode":"humidity","value":50}}
    char *js_str = cJSON_Print(root);
    strcpy(system_str, js_str);
    cJSON_Delete(root);
    cJSON_free(js_str);
}

extern uint8_t ave;
void get_str_temp_log(char *temp_log_str)
{
    cJSON *root;
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "temp_in", sensors_data->in);
    cJSON_AddNumberToObject(root, "temp_out", sensors_data->out);
    cJSON_AddNumberToObject(root, "temp_ave", ave);
    cJSON_AddNumberToObject(root, "Kp", device->extra_options.Kp);
    cJSON_AddNumberToObject(root, "Ki", device->extra_options.Ki);
    cJSON_AddNumberToObject(root, "Kd", device->extra_options.Kd);
    cJSON_AddNumberToObject(root, "temp_limit", device->temp_limit);
    cJSON_AddNumberToObject(root, "ten_power", device->ten_power);
    cJSON_AddNumberToObject(root, "ten_state", device->ten_state);

    char *js_str = cJSON_Print(root);
    strcpy(temp_log_str, js_str);
    cJSON_Delete(root);
    cJSON_free(js_str);
}

void get_str_extra_options(char *extra)
{
    cJSON *root, *extra_js;
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "extra_options", extra_js = cJSON_CreateObject());
    cJSON_AddNumberToObject(extra_js, "deviation", device->extra_options.deviation);
    cJSON_AddNumberToObject(extra_js, "check_time", device->extra_options.check_time);
    cJSON_AddNumberToObject(extra_js, "step_pwm", device->extra_options.step_pwm);

    cJSON_AddNumberToObject(extra_js, "Kp", device->extra_options.Kp);
    cJSON_AddNumberToObject(extra_js, "Ki", device->extra_options.Ki);
    cJSON_AddNumberToObject(extra_js, "Kd", device->extra_options.Kd);
    cJSON_AddNumberToObject(extra_js, "ten_state", device->ten_state);

    char *js_str = cJSON_Print(root);
    strcpy(extra, js_str);
    cJSON_Delete(root);
    cJSON_free(js_str);
}
