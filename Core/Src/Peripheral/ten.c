#include "ten.h"
#include "tim.h"
#include "damper.h"
#include "math.h"
#include "utils.h"
#include "mqtt_message.h"

uint16_t _ten_power;

float P, I, D, En;
float En1 = 0;

int constrain(int X, int min, int max)
{
    if (X > max)
    {
        return max;
    }
    else if (X < min)
    {
        return min;
    }
    else
    {
        return X;
    }
}

void set_ten_power(uint16_t power)
{
	if (_ten_power != power)
	{
        if (power > 0)
        {
        	HAL_GPIO_WritePin (RELAY_CH2_GPIO_Port, RELAY_CH2_Pin, ON);
        }
        else
        {
        	HAL_GPIO_WritePin (RELAY_CH2_GPIO_Port, RELAY_CH2_Pin, OFF);
        }
        _ten_power = power;
		__HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_2, power * 65535 / 1023);
		DEBUG_TEN("Set power = %i \n", power);
	}
}

int computePID_true(float Xn, float X0n, float Kp, float Ki, float Kd, float dt, int min, int max)
{
    En = X0n - Xn;
    P = Kp * En;
    I = constrain(I + Ki * En, min, max);
    D = Kd * (En - En1) / dt;
    En1 = En;
    return constrain(P + I + D, min, max);
}

void ten_handler()
{
	if (device->state == ON && device->ten_state == ON && damper_state == 2 && device->error_ds18b20 == false &&
		device->error_temp_hot == false && device->error_stop_hot == false && device->error_stop_cold == false)
	{
//		device->ten_power = computePID_true(sensors_data->out, device->temp_limit, 75, 3.0, 13, 3, 0, 1023);
		device->ten_power = computePID_true(sensors_data->out, device->temp_limit, (float)device->extra_options.Kp / 10, (float)device->extra_options.Ki / 10, (float)device->extra_options.Kd / 10, 3, 0, 1023);
		DEBUG_TEN("work mode\n");
	}
	else
	{
		device->ten_power = 0;
		DEBUG_TEN("ten off\n");
	}
	set_ten_power(device->ten_power);
}
