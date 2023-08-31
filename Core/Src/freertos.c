/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lwip.h"
#include "lwip/api.h"
#include "mqtt_client.h"
//#include "httpserver.h"
#include "remote_control.h"
#include "led_button_control.h"
#include "mqtt_message.h"
#include "fan.h"
#include "usart.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern struct netif gnetif;
/////////////////////////////////
osThreadId_t ledsTaskHandle;
const osThreadAttr_t ledsTask_attributes = {
  .name = "ledsTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 256 * 4
};

osThreadId_t damperTaskHandle;
const osThreadAttr_t damperTask_attributes = {
  .name = "damperTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 256 * 4
};
////////////////////////////////
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
extern void LedsTask(void *argument);
extern void DamperTask(void *argument);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

extern void MX_LWIP_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
//  ledsTaskHandle = osThreadNew(LedsTask, NULL, &ledsTask_attributes);
//  damperTaskHandle = osThreadNew(DamperTask, NULL, &damperTask_attributes);
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN StartDefaultTask */
//  init_mqtt();
//  http_server_init();
  check_remote_control();
  /* Infinite loop */
  for(;;)
  {
//	  inst_speed = device->speed_arr[device->inflow_speed];
//	  set_inflow_fan1(inst_speed);

//	set_inflow_fan1(0);
//	osDelay(2000);
//	set_inflow_fan1(10);
//	osDelay(2000);
//	set_inflow_fan1(20);
//	osDelay(2000);
//	set_inflow_fan1(30);
//	osDelay(2000);
//	set_inflow_fan1(40);
//	osDelay(2000);
//	set_inflow_fan1(50);
//	osDelay(2000);
//	set_inflow_fan1(100);
//	osDelay(2000);
//	HAL_GPIO_TogglePin(RELAY_CH2_GPIO_Port, RELAY_CH2_Pin);
//	HAL_GPIO_TogglePin(RELAY_CH3_GPIO_Port, RELAY_CH3_Pin);
//	HAL_GPIO_TogglePin(RELAY_CH4_GPIO_Port, RELAY_CH4_Pin);
//	HAL_GPIO_TogglePin(HEAT_VALVE_GATE1_GPIO_Port, HEAT_VALVE_GATE1_Pin);
//	HAL_GPIO_TogglePin(SSR_GATE1_GPIO_Port, SSR_GATE1_Pin);
//	printf("HELLO WORLD\n");
	  uint8_t ert = 5;
//	HAL_UART_Transmit_IT(&huart4, sprintf("ert = %i\n", ert), 18);
//	  HAL_UART_Transmit_IT(&huart4, "HELLOOOO\n", 18);
//	  HAL_UART_Transmit_IT(&huart4, str, strlen(str));
//	  DEBUG_MAIN("HELLO WORLD\n");
//	start_mqtt();
//		char msg[15];
//		sprintf(msg, "speed %i\n", ert);  // convert int16_t to ascii, legal itoa
//		HAL_UART_Transmit_IT(&huart4, (uint8_t *) msg, 14);



    osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

