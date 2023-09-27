#include "led_button_control.h"
#include "storage.h"
#include "cmsis_os.h"
#include "netif.h"

extern struct netif gnetif;

uint32_t  timme = 0;
uint8_t service_response = 0;

bool link_IP;
bool reset_led;

void button_handler()
{
	if(HAL_GPIO_ReadPin(SERVICE_BTN_GPIO_Port, SERVICE_BTN_Pin) == GPIO_PIN_RESET)
	{
		timme = HAL_GetTick();
	}

	if(HAL_GPIO_ReadPin(SERVICE_BTN_GPIO_Port, SERVICE_BTN_Pin) == GPIO_PIN_SET)
	{
		if(service_response == 1 && ((HAL_GetTick() - timme) < 5000))
		{
			service_response = 3;
		}
		if((HAL_GetTick() - timme) > 5000)
		{
			if(service_response == 1)
			{
				service_response = 4;
			}
			if(service_response == 0)
			{
				service_response = 1;
				timme = HAL_GetTick();
				HAL_GPIO_WritePin(LED_LINK_GPIO_Port, LED_LINK_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED_WORK_GPIO_Port, LED_WORK_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED_FAILURE_GPIO_Port, LED_FAILURE_Pin, GPIO_PIN_RESET);
			}
		}
	 }
}

void link_callback_IP()
{
    if (link_IP == true && service_response == 0)
    	HAL_GPIO_TogglePin(LED_LINK_GPIO_Port, LED_LINK_Pin);
    if (service_response == 3)
    	HAL_GPIO_TogglePin(LED_WORK_GPIO_Port, LED_WORK_Pin);
    if (service_response == 4)
    	HAL_GPIO_TogglePin(LED_FAILURE_GPIO_Port, LED_FAILURE_Pin);
}

void start_failure_led()
{
	if (device->error_temp_hot || device->error_temp_cold || device->error_stop_hot || device->error_stop_cold)
		HAL_GPIO_WritePin(LED_FAILURE_GPIO_Port, LED_FAILURE_Pin, GPIO_PIN_SET);
    else
    	HAL_GPIO_WritePin(LED_FAILURE_GPIO_Port, LED_FAILURE_Pin, GPIO_PIN_RESET);
}

void start_work_led()
{
	if(mqtt_status)
		HAL_GPIO_WritePin(LED_WORK_GPIO_Port, LED_WORK_Pin, GPIO_PIN_SET);
    else
    	HAL_GPIO_WritePin(LED_WORK_GPIO_Port, LED_WORK_Pin, GPIO_PIN_RESET);
}

void start_link_led()
{
	if(netif_is_link_up(&gnetif))
    {
        if(mqtt_status)
        {
        	link_IP = false;
        	HAL_GPIO_TogglePin(LED_LINK_GPIO_Port, LED_LINK_Pin);
        }
        else
        	link_IP = true;
    }
    else
    	HAL_GPIO_WritePin(LED_LINK_GPIO_Port, LED_LINK_Pin, GPIO_PIN_RESET);
}

void start_service_led()
{
	if(service_response == 1)//вошли в сервисный режим
    {
		DEBUG_LED_BUT_CONTROL("service_response = 1\n");
		HAL_GPIO_TogglePin(LED_LINK_GPIO_Port, LED_LINK_Pin);
		HAL_GPIO_TogglePin(LED_WORK_GPIO_Port, LED_WORK_Pin);
		HAL_GPIO_TogglePin(LED_FAILURE_GPIO_Port, LED_FAILURE_Pin);
    }
    if(service_response == 3)//сброс ошибок
    {
    	DEBUG_LED_BUT_CONTROL("service_response = 3\n");
    	HAL_GPIO_WritePin(LED_LINK_GPIO_Port, LED_LINK_Pin, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(LED_FAILURE_GPIO_Port, LED_FAILURE_Pin, GPIO_PIN_RESET);
    }
    if(service_response == 4)//сброс памяти
    {
    	DEBUG_LED_BUT_CONTROL("service_response = 4\n");
    	HAL_GPIO_WritePin(LED_LINK_GPIO_Port, LED_LINK_Pin, GPIO_PIN_RESET);
    	HAL_GPIO_WritePin(LED_WORK_GPIO_Port, LED_WORK_Pin, GPIO_PIN_RESET);
    }
	if((service_response > 0) && ((HAL_GetTick() - timme) > 10000))
		service_response = 0;
}

void LedsTask(void *argument)
{
    for (;;)
    {
    	if (service_response == 0)
    	{
            start_link_led();
            start_work_led();
            start_failure_led();
    	}
    	else
    		start_service_led();
        osDelay(1000);
    }
}
