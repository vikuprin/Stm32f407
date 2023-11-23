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
#include "httpserver.h"
#include "led_button_control.h"
#include "mqtt_message.h"
#include "fan.h"
#include "modes.h"
#include "usart.h"
#include "string.h"
#include "lwip/dns.h"
#include "lwip/apps/mdns.h"
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

osThreadId ledsTaskHandle;
osThreadId damperTaskHandle;
osThreadId mainTaskHandle;
osThreadId dsTaskHandle;
osThreadId otaTaskHandle;

bool mdns_set = false;
bool mdns_search = false;
uint8_t mdns_id = 1;
char *mdns_name;
static void srv_txt(struct mdns_service *service, void *txt_userdata)
{
	err_t res;
    LWIP_UNUSED_ARG(txt_userdata);
    DEBUG_MAIN("mdns\n");
    res = mdns_resp_add_service_txtitem(service, "path=/", 6);
    LWIP_ERROR("mdns add service txt failed\n", (res == ERR_OK), return);
}

void dns_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg)
{
	DEBUG_MAIN("dns callback\n");
    if (ipaddr != NULL)
    {
    	DEBUG_MAIN("name = %s\n", name);
    	DEBUG_MAIN("ip = %s\n", ipaddr_ntoa(ipaddr));
    	sprintf(mdns_name, "cityair350%d", mdns_id);
    	mdns_id++;
    	mdns_search = false;
    }
    else
    {
    	DEBUG_MAIN("Not found dns ip %s\n", mdns_name);
		mdns_resp_add_netif(&gnetif, mdns_name, 120);
		mdns_resp_add_service(&gnetif, mdns_name, "_http", DNSSD_PROTO_TCP, 80, 3600, srv_txt, NULL); //mdns_name
		mdns_set = true;
		mdns_search = false;
    }
}
/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
extern void LedsTask(void const * argument);
extern void DamperTask(void const * argument);
extern void MainTask(void const * argument);
extern void DSTask(void const * argument);
extern void OtaTask(void const * argument);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

extern void MX_LWIP_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/* USER CODE BEGIN GET_TIMER_TASK_MEMORY */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
  *ppxTimerTaskStackBuffer = &xTimerStack[0];
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
  /* place for user code */
}
/* USER CODE END GET_TIMER_TASK_MEMORY */

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
  /* definition and creation of defaultTask */

  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 1024);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
//  osThreadDef(mainTask, MainTask, osPriorityNormal, 0, 1024);
//  mainTaskHandle = osThreadCreate(osThread(mainTask), NULL);
//
//  osThreadDef(ledsTask, LedsTask, osPriorityNormal, 0, 256);
//  ledsTaskHandle = osThreadCreate(osThread(ledsTask), NULL);
//
//  osThreadDef(damperTask, DamperTask, osPriorityNormal, 0, 256);
//  damperTaskHandle = osThreadCreate(osThread(damperTask), NULL);
//
//  osThreadDef(dsTask, DSTask, osPriorityNormal, 0, 1024);
//  dsTaskHandle = osThreadCreate(osThread(dsTask), NULL);

  osThreadDef(otaTask, OtaTask, osPriorityRealtime, 0, 3 * 1024);
  otaTaskHandle = osThreadCreate(osThread(otaTask), NULL);
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
extern bool ota_flag;
void StartDefaultTask(void const * argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN StartDefaultTask */
  while (dhcp_supplied_address(&gnetif) == 0)
  {
     osDelay(50);
  }
  #if DNS == 1
  mdns_name = malloc(80);
  sprintf(mdns_name, "cityair350");
  ip_addr_t addr;
  while (mdns_set == false)
  {
	  dns_gethostbyname(mdns_name, &addr, dns_callback, NULL);
	  mdns_search = true;
	  while (mdns_search == true)
	  {
		  osDelay(100);
	  }
  }
  #endif
  http_server_init();
  init_mqtt();
  device_send = *device;
  device_check = *device;
  /* Infinite loop */
  for(;;)
  {
	  if (!ota_flag)
		  start_mqtt();
	  osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
