#include "damper.h"
#include "sensor.h"
#include "cmsis_os.h"

#define OPENING_TIME (45 * SECOND)

uint32_t damper_time;

void open_damper()
{
    if (damper_state == 0)
    {
    	damper_state = 1;
    	damper_time = HAL_GetTick();
    	HAL_GPIO_WritePin(RELAY_CH1_GPIO_Port, RELAY_CH1_Pin, GPIO_PIN_SET);
        DEBUG_DAMP("opens damper\n");
    }
}

void close_damper()
{
	damper_state = 0;
	HAL_GPIO_WritePin(RELAY_CH1_GPIO_Port, RELAY_CH1_Pin, GPIO_PIN_RESET);
	DEBUG_DAMP("close damper\n");
}

void DamperTask(void *argument)
{
	damper_state = 0;
    for(;;)
    {
    	if(device->state == ON && device->error_stop_hot == false && device->error_stop_cold == false && device->error_ds18b20 == false &&
            sensors_data->out_state && sensors_data->in_state && ds_count == 2 && device->error_fan == false)
    	{
    		open_damper();
    		if(damper_state == 1 && ((HAL_GetTick() - damper_time) > OPENING_TIME))
    		{
    			damper_state = 2;
    			DEBUG_DAMP("opened damper\n");
    		}
    	}
        else
        	close_damper();
        osDelay(1000);
    }
}
