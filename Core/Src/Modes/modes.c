#include "modes.h"
#include "sensor.h"
#include "ten.h"
#include "damper.h"
#include "fan.h"
#include "cmsis_os.h"
#include "mqtt_client.h"
#include "remote_control.h"

#define HOT_TEMP 60
#define MAX_FAN_COUNT 10

extern uint8_t ave;

bool temp_cold_state = false;
uint8_t cold_speed;
bool inflow_max_flag;
uint64_t cold_time;
uint64_t hot_time;
uint64_t inflow_max_time;

void temp_cold()
{
    if (device->error_stop_cold == true && ((HAL_GetTick() - cold_time) > 60 * MINUTE))
    {
    	cold_time = HAL_GetTick();
        device->error_stop_cold = false;
    }
    if(device->error_stop_cold == false && ((HAL_GetTick() - cold_time) > device->extra_options.check_time * MINUTE))
    {
    	cold_time = HAL_GetTick();
        DEBUG_MODES("callback_temp_cold_timer\n");
        if (device->state == ON	&& damper_state == 2 && sensors_data->out_state && sensors_data->in_state && inflow_max_flag == false &&
            device->inflow_speed > 0 && (ave < (device->temp_limit - device->extra_options.deviation)) && (device->ten_power >= 970)) // должно быть 970
        {
            if (temp_cold_state == false)
            {
                temp_cold_state = true;
                cold_speed = device->speed_arr[device->inflow_speed];
                DEBUG_MODES("ZAFIKSIROVAL SKOROST\n");
            }
            if (cold_speed <= device->speed_arr[1])
            {
                if (ave < (10 - device->extra_options.deviation)) // должно быть 10
                {
                    device->error_stop_cold = true;
                    device->error_temp_cold = false;
                    cold_time = HAL_GetTick();
                    DEBUG_MODES("ERROR_STOP\n");
                    DEBUG_MODES("ERROR_STOP\n");
                    DEBUG_MODES("ERROR_STOP\n");
                }
                else
                {
                    cold_speed = device->speed_arr[1];
                    DEBUG_MODES("PERVAYA SKOROST\n");
                }
            }
            else
            {
                cold_speed = cold_speed - device->extra_options.step_pwm;
                DEBUG_MODES("UBAVILI NA 5\n");
            }
            device->error_temp_cold = true;
            DEBUG_MODES("error_temp_cold\n");
        }
        else
        {
            if (temp_cold_state)
            {
                cold_speed = cold_speed + device->extra_options.step_pwm;
                DEBUG_MODES("PRIBAVILI NA 5\n");
                if (cold_speed >= device->speed_arr[device->inflow_speed])
                {
                    cold_speed = device->speed_arr[device->inflow_speed];
                    temp_cold_state = false;
                    DEBUG_MODES("SRAVNYALI ISHODNOE ZNACHENIE\n");
                    device->error_temp_cold = false;
                    DEBUG_MODES("VSE OK\n");
                }
            }
            else
            {
                device->error_temp_cold = false;
                DEBUG_MODES("VSE OK\n");
            }
        }
    }
}

void temp_hot()
{
	if (device->error_temp_hot == true && ((HAL_GetTick() - hot_time) > 3 * MINUTE))
	{
	    device->error_stop_hot = true;
	    device->error_temp_hot = false;
	    DEBUG_MODES("ERROR_STOP\n");
	    DEBUG_MODES("ERROR_STOP\n");
	    DEBUG_MODES("ERROR_STOP\n");
	}
    if (device->error_stop_hot == false && ave >= HOT_TEMP) // должно быть 60
    {
        device->error_temp_hot = true;
        hot_time = HAL_GetTick();
        DEBUG_MODES("error_temp_hot\n");
    }
}

void inflow_mode()
{
    if (device->error_temp_hot == false && device->error_temp_cold == false && device->error_stop_hot == false && device->error_stop_cold == false)
    {
        inst_speed = device->speed_arr[device->inflow_speed];
    }
    if (device->error_temp_hot == true && device->error_stop_hot == false)
    {
        inst_speed = MAXIMUM;
        DEBUG_MODES("speed = MAXIMUM\n");
    }
    if (device->error_temp_cold == true && device->error_stop_cold == false)
    {
        inst_speed = cold_speed;
        DEBUG_MODES("speed = cold speed\n");
    }
    if (device->error_stop_hot || device->error_stop_cold)
    {
        inst_speed = 0;
        DEBUG_MODES("finish\n");
    }
    DEBUG_MODES("inst_speed %i\n", inst_speed);
    set_inflow_fan1(inst_speed);
}

void inflow_max_mode()
{
    if (!inflow_max_flag)
    {
        inflow_max_flag = true;
        inflow_max_time = HAL_GetTick();
        set_inflow_fan1(device->speed_arr[7]);
        DEBUG_MODES("Start inflow max mode\n");
    }
    if (HAL_GetTick() - inflow_max_time > INFLOW_MAX_MODE_TIME)
    {
        device->mode = device->last_mode;
        DEBUG_MODES("Set last mode = %i\n", device->mode);
    }
    if (inflow_max_time > HAL_GetTick())
    {
        device->mode = device->last_mode;
        DEBUG_MODES("TIME RESET Set last mode = %i\n", device->mode);
    }

}

void mode_handler()
{
    switch (device->mode)
    {
    case INFLOW_MODE:
        inflow_max_flag = false;
        inflow_mode();
//        reset_smart_mode();
        break;
    case SMART_MODE:
        inflow_max_flag = false;
        if (device->smart.set == true)
            smart_mode_external();
//        else
//            smart_mode_internal();
        break;
    case INFLOW_MAX_MODE:
        inflow_max_mode();
        break;
    default:
        break;
    }
}

void pcnt_error()
{
	if(inflow_power1 > 0)
	{
		fan_count++;
		if(fan_count == MAX_FAN_COUNT)
			device->error_fan = true;
		else
			device->error_fan = false;
	}
	else
		fan_count = 0;
}

void inflow_handler()
{
    if (device->state == ON && damper_state == 2 && device->error_ds18b20 == false)
    {
        mode_handler();
        pcnt_error();
    }
    else
    	set_inflow_fan1(OFF);
}

void MainTask(void const * argument)
{
    DEBUG_MODES("DEVICE STATE %i\n", device->state);
    DEBUG_MODES("DEVICE MODE %i\n", device->mode);
    DEBUG_MODES("DEVICE inflow_speed %i\n", device->inflow_speed);
    DEBUG_MODES("speed {%i,%i,%i,%i,%i,%i,%i,%i}\n", device->speed_arr[0], device->speed_arr[1], device->speed_arr[2], device->speed_arr[3], device->speed_arr[4], device->speed_arr[5], device->speed_arr[6], device->speed_arr[7]);
    osDelay(3000);
    check_remote_control();
    for(;;)
    {
    	temp_hot();  // функция проверки перегрева
    	temp_cold(); // функция проверки переохлаждения
    	inflow_handler();
        osDelay(300);
    }
}
