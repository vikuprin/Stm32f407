#include "utils.h"
#include "../main.h"
#include "string.h"

int map(int x, int in_min, int in_max, int out_min, int out_max)
{
    const long dividend = out_max - out_min;
    const long divisor = in_max - in_min;
    const long delta = x - in_min;

    return (delta * dividend + (divisor / 2)) / divisor + out_min;
}

bool check_js_param_int(cJSON *data_json, char *key, int *set_value)
{
    cJSON *param = cJSON_GetObjectItem(data_json, key);
    if (param != NULL)
    {
        *set_value = param->valueint;
        DEBUG_UTILS("Set param %s = %i\n", key, param->valueint);
        return true;
    }
    return false;
}

bool check_js_param_u8(cJSON *data_json, char *key, uint8_t *set_value)
{
    cJSON *param = cJSON_GetObjectItem(data_json, key);
    if (param != NULL)
    {
        *set_value = param->valueint;
        DEBUG_UTILS("Set param %s = %i\n", key, param->valueint);
        return true;
    }
    return false;
}
bool check_char_mode(cJSON *data_json, char *key, uint8_t *set_value)
{
    cJSON *param = cJSON_GetObjectItem(data_json, key);
    if (param != NULL)
    {
        if (strcmp(param->valuestring, "inflow") == 0)
            *set_value = INFLOW_MODE;
        if (strcmp(param->valuestring, "smart") == 0)
            *set_value = SMART_MODE;
        if (strcmp(param->valuestring, "inflow_max") == 0)
        {
            if (device->mode != INFLOW_MAX_MODE)
                device->last_mode = device->mode;
            *set_value = INFLOW_MAX_MODE;
        }

        DEBUG_UTILS("Set param %s = %s\n", key, param->valuestring);
        return true;
    }
    return false;
}
bool check_char_on_off(cJSON *data_json, char *key, uint8_t *set_value)
{
    cJSON *param = cJSON_GetObjectItem(data_json, key);
    if (param != NULL)
    {
        if (strcmp(param->valuestring, "on") == 0)
            *set_value = 1;
        if (strcmp(param->valuestring, "off") == 0)
            *set_value = 0;
        DEBUG_UTILS("Set param %s = %s\n", key, param->valuestring);
        return true;
    }
    return false;
}

bool check_js_param_double(cJSON *data_json, char *key, double *set_value)
{
    cJSON *param = cJSON_GetObjectItem(data_json, key);
    if (param != NULL)
    {
        *set_value = param->valuedouble;
        DEBUG_UTILS("Set param %s = %0.1f\n", key, param->valuedouble);
        return true;
    }
    return false;
}
bool check_js_param_u16(cJSON *data_json, char *key, uint16_t *set_value)
{
    cJSON *param = cJSON_GetObjectItem(data_json, key);
    if (param != NULL)
    {
        *set_value = param->valueint;
        DEBUG_UTILS("Set param %s = %i\n", key, param->valueint);
        return true;
    }
    return false;
}

bool check_js_param_char(cJSON *data_json, char *key, char *set_value)
{
    cJSON *param = cJSON_GetObjectItem(data_json, key);
    if (param != NULL)
    {
        strcpy(set_value, param->valuestring);
        DEBUG_UTILS("Set param %s = %s\n", key, param->valuestring);
        return true;
    }
    return false;
}
