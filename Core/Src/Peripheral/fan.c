#include "fan.h"
#include "tim.h"

void set_inflow_fan1(uint8_t value)
{
	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, value * 65535 / 100);
}
