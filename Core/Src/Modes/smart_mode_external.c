#include "smart_mode_external.h"
#include "mqtt_client.h"
#include "fan.h"
#include "ten.h"
#include "utils.h"
#include "modes.h"

extern TIM_HandleTypeDef htim4;
extern bool sub_request_cb;

float P_k, I_k, D_k, E_n;
float E_n1 = 0, E_n2 = 0;
bool smart_mode_state;
int last_delta;

void reset_smart_mode()
{
    if (smart_mode_state == ON)
    {
    	DEBUG_MODES("RESET SMART MODE!!!");
        smart_mode_state = OFF;
    }
}

bool get_smart_mode_state()
{
    return smart_mode_state;
}

int compute_speed_pwm(float Xn, float X0n, float Kp, float Ki, float Kd, float dt, int min, int max)
{
    E_n = Xn - X0n;
    P_k = Kp * E_n;
    I_k = constrain(I_k + Ki * E_n, min, max);
    D_k = Kd * (E_n - E_n1) / dt;

    E_n1 = E_n;

    return constrain(P_k + I_k + D_k, min, max);
}

void calc_smart_speed()
{
    device->smart_speed_pwm = compute_speed_pwm(device->smart.value / 10, device->smart.limit / 10, 0.1, 0.1, 7, 180, device->speed_arr[1], 100);
    DEBUG_MODES("Calc delta_from_delta %i", device->smart_speed_pwm);
}

void start_smart_mode()
{
	DEBUG_MODES("Smart mode active");
    smart_mode_state = ON;
    device->smart_speed_pwm = compute_speed_pwm(device->smart.value / 10, device->smart.limit / 10, 0.1, 0.1, 7, 10, device->speed_arr[1], 100);
    HAL_TIM_Base_Start_IT(&htim4);
}

void off_smart_mode()
{
    device->smart_speed_pwm = 0;
    if (smart_mode_state == ON)
    {
    	DEBUG_MODES("Smart mode disable");
    	HAL_TIM_Base_Stop(&htim4);
        smart_mode_state = OFF;
    }
}

void smart_mode_external()
{
    if ((device->error_temp_hot == false && device->error_temp_cold == false && device->error_stop_hot == false && device->error_stop_cold == false) 
        && device->smart.value > device->smart.limit && device->smart.set && sub_request_cb)
    {
        if (smart_mode_state != ON)
        {
            start_smart_mode();
        }
    }
    else
    {
        off_smart_mode();
    }

    if (device->error_temp_hot == true && device->error_stop_hot == false)
    {
    	set_inflow_fan1(MAXIMUM);
    	DEBUG_MODES("EXT SMART speed = MAXIMUM");
    }
    else if (device->error_temp_cold == true && device->error_stop_cold == false)
    {
    	set_inflow_fan1(cold_speed);
    	DEBUG_MODES("EXT SMART speed = cold speed");
    }
    else if (device->error_stop_hot || device->error_stop_cold)
    {
    	set_inflow_fan1(0);
    	DEBUG_MODES("EXT SMART finish");
    }
    else
    {
    	set_inflow_fan1(device->smart_speed_pwm);
    }
    DEBUG_MODES("EXTERNAL SMART SPEED = %i", device->smart_speed_pwm);
}
