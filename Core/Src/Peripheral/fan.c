#include "fan.h"
#include "tim.h"

void set_inflow_fan1(uint8_t value)
{
    TIM_OC_InitTypeDef sConfigOC;
    inflow_power1 = value;
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = value * 65535 / 100;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_2); // таймер №8, канал №2
    HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
}
