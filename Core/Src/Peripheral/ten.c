#include "ten.h"
#include "tim.h"
#include "damper.h"
#include "math.h"
#include "utils.h"

uint8_t _ten_power;

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

void set_ten_power(uint8_t power)
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
		__HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_2, power); // * 65535 / 100);
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
	if (device->state == ON && damper_state == 2 &&
		heaters->on_off_ten == ON && device->inflow_speed > 0 && sensors_data->in_state && sensors_data->out_state &&
		device->error_temp_hot == false && device->error_stop_hot == false && device->error_stop_cold == false)
	{
		heaters->ten.power = computePID_true(sensors_data->out, heaters->ten.temp_limit, 75, 3.0, 13, 1, 0, 65535);
		DEBUG_TEN("work mode\n");
	}
	else
	{
		heaters->ten.power = 0;
		DEBUG_TEN("ten off\n");
	}
	set_ten_power(heaters->ten.power);
	// publish_temp_log();
	DEBUG_TEN("TEN_POWER %i\n", heaters->ten.power);
}
