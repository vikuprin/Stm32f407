#include "damper.h"
#include "sensor.h"
#include "cmsis_os2.h"

#define OPENING_TIME 45000

uint32_t timer_damper;

void open_damper()
{
    if (state_damper == 0)
    {
    	state_damper = 1;
    	timer_damper = HAL_GetTick(); // запуск таймера
    	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET);
//        DEBUG_DAMP("opens damper\n");
    }
}

void close_damper()
{
	state_damper = 0;
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);
//	DEBUG_DAMP("close damper\n");
}

void DamperTask(void *argument)
{
    for(;;)
    {
    	if(device->state == ON && device->error_stop_hot == false && device->error_stop_cold == false &&
            sensors_data->out_state && sensors_data->in_state && device->error_ds18_bus == false && device->error_ds18_lack == false && ds_count == 2)
    	{
    		open_damper();
    		if(state_damper == 1 && ((HAL_GetTick() - timer_damper) > OPENING_TIME))
    		{
    			state_damper = 2;
//    			DEBUG_DAMP("opened damper\n");
    		}
    	}
        else
        	close_damper();
        osDelay(1000);
    }
}
