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
    cJSON_AddNumberToObject(capabilities, "heat", heaters->ten.temp_limit);
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

void get_str_settings(char *settings)
{
    cJSON *root, *settings_js, *ten;
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings_js = cJSON_CreateObject());
    cJSON_AddItemToObject(settings_js, "ten", ten = cJSON_CreateObject());
    cJSON_AddNumberToObject(ten, "temp_limit", heaters->ten.temp_limit);
    cJSON_AddNumberToObject(ten, "on_off", heaters->on_off_ten);
    cJSON_AddNumberToObject(ten, "koef_grow", heaters->koef_grow);
    cJSON_AddNumberToObject(ten, "update_koef_time", heaters->update_koef_time);
    cJSON_AddNumberToObject(ten, "delta", heaters->delta);

    char *js_str = cJSON_Print(root);
    strcpy(settings, js_str);
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
    cJSON_AddNumberToObject(errors_js, "ds18_bus", device->error_ds18_bus);
    cJSON_AddNumberToObject(errors_js, "ds18_lack", device->error_ds18_lack);

    char *js_str = cJSON_Print(root);
    strcpy(errors, js_str);
    cJSON_Delete(root);
    cJSON_free(js_str);
}

void get_str_system_status(char *system_str)
{
    cJSON *root;
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "heat_koef", heaters->koef);
    cJSON_AddNumberToObject(root, "ten_power", heaters->ten.power);
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

void get_str_temp_log(char *temp_log_str)
{
    cJSON *root;
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "device_state", device->state);
    cJSON_AddNumberToObject(root, "temp_out", sensors_data->out);
    cJSON_AddNumberToObject(root, "temp_out_real", sensors_data->out_real);
    cJSON_AddNumberToObject(root, "temp_in", sensors_data->in);
    cJSON_AddNumberToObject(root, "heat_koef", heaters->ten.power);
    cJSON_AddNumberToObject(root, "temp_limit", heaters->ten.temp_limit);
    cJSON_AddNumberToObject(root, "speed", device->speed_arr[device->inflow_speed]);
    cJSON_AddNumberToObject(root, "inst_speed", inst_speed);

    // {"entity_on_off_dependence":"off","entity_dependence":{"device_id_master":0,"mode":"humidity","value":50}}
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

    char *js_str = cJSON_Print(root);
    strcpy(extra, js_str);
    cJSON_Delete(root);
    cJSON_free(js_str);
}
