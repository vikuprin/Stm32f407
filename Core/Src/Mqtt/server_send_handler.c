#include "send_server_handler.h"
#include "mqtt_message.h"
#include "mqtt_client.h"
#include "mqtt_objects.h"
#include "math.h"

char *modes_str_send[2] = {"inflow_mode", "inflow_max_mode"};
char *on_off_str_send[2] = {"off", "on"};
char speeds_str_send[10];
char temp_str_send[30];
bool send_array_device[10] = {0};
bool send_array_entity[10][5];
bool start_timer_f;
uint32_t  server_time = 0;

void publish_from_timer()
{
    start_timer_f = false;
    DEBUG_SEND("Timer done!\n");
    if (wireless_params->mqtt_type != VAKIO_MQTT)
    {
        if (device_send.mode != device_check.mode)
        {
            iSendMode = true;
            publish_message(workmode_topic, modes_str_send[device->mode]);
            device_send.mode = device_check.mode;
        }
        if (device_send.state != device_check.state)
        {
            iSendState = true;
            publish_message(state_topic, on_off_str_send[device->state]);
            device_send.state = device_check.state;
        }
        if (device_send.inflow_speed != device_check.inflow_speed)
        {
            iSendSpeed = true;
            sprintf(speeds_str_send, "%i", device->inflow_speed);
            publish_message(speed_topic, speeds_str_send);
            device_send.inflow_speed = device_check.inflow_speed;
        }
        if (heaters_send.ten.temp_limit != heaters_check.ten.temp_limit)
        {
            iSendTemp = true;
            sprintf(temp_str_send, "%i", heaters->ten.temp_limit);
            publish_message(temp_limit_topic, temp_str_send);
            heaters_send.ten.temp_limit = heaters_check.ten.temp_limit;
        }
    }

    if (send_array_device[CAPABILITIES])
    {
        publish_capabilities();
        send_array_device[CAPABILITIES] = false;
    }
    if (send_array_device[SETTINGS])
    {
        send_array_device[SETTINGS] = false;
        publish_settings();
    }
    if (send_array_device[FAULTS])
    {
        send_array_device[FAULTS] = false;
        publish_errors();
    }
}

bool check_param_u8(uint8_t *param_send, uint8_t *param, char *key)
{
    bool check = false;
    if (*param_send != *param)
    {
        DEBUG_SEND(" Param |%s| not equal { %i != %i }\n", key, *param_send, *param);
        *param_send = *param;
        check = true;
        if (!start_timer_f)
        {
            DEBUG_SEND("Timer start 5 sec\n");
            start_timer_f = true;
            server_time = HAL_GetTick();
        }
        else
        {
            DEBUG_SEND("Timer restart 5 sec\n");
            server_time = HAL_GetTick();
        }
    }
    return check;
}

void send_server_task()
{
	if (start_timer_f && (HAL_GetTick() - server_time) > 5000)
		publish_from_timer();

    if (check_param_u8(&device_check.state, &device->state, "State") ||
        check_param_u8(&device_check.inflow_speed, &device->inflow_speed, "Speed") ||
        check_param_u8(&device_check.mode, &device->mode, "Mode") ||
        check_param_u8(&heaters_check.ten.temp_limit, &heaters->ten.temp_limit, "Temp"))

        send_array_device[CAPABILITIES] = true;

    if (check_param_u8(&device_check.error_temp_hot, &device->error_temp_hot, "temp_hot") ||
        check_param_u8(&device_check.error_stop_hot, &device->error_stop_hot, "stop_hot") ||
        check_param_u8(&device_check.error_temp_cold, &device->error_temp_cold, "temp_cold") ||
        check_param_u8(&device_check.error_stop_cold, &device->error_stop_cold, "stop_cold") ||
        check_param_u8(&device_check.error_ds18_bus, &device->error_ds18_bus, "DS18_bus") ||
        check_param_u8(&device_check.error_ds18_lack, &device->error_ds18_lack, "DS18_lack") ||
        check_param_u8(&device_check.remote_control.error, &device->remote_control.error, "remote_control"))

        send_array_device[FAULTS] = true;
}
