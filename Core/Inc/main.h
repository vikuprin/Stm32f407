/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "stdbool.h"
#include "usart.h"

#define DEBUG_PRINT 1

uint8_t msg[80];
#if DEBUG_PRINT == 1
#define DEBUG_MAIN(...) printf("MAIN: "__VA_ARGS__);
#elif DEBUG_PRINT == 2
#define DEBUG_MAIN(...) sprintf(msg, "MAIN: "__VA_ARGS__);HAL_UART_Transmit_IT(&huart4, (uint8_t*)msg, strlen(msg));
#else
#define DEBUG_MAIN(...)
#endif

#define BOOTLOADER 1

#define BOOT_ADDR_FLASH      0x08000000  //FLASH_SECTOR_0

#define MAIN_ADDR_FLASH_1    0x08040000  //FLASH_SECTOR_6
#define MAIN_ADDR_FLASH_2    0x08060000  //FLASH_SECTOR_7
#define MAIN_ADDR_FLASH_3    0x08080000  //FLASH_SECTOR_8

#define OTA_ADDR_FLASH_1     0x080A0000  //FLASH_SECTOR_9
#define OTA_ADDR_FLASH_2     0x080C0000  //FLASH_SECTOR_10
#define OTA_ADDR_FLASH_3     0x080E0000  //FLASH_SECTOR_11

#define CHECK_EXT_BYTE       0
#define OTA_LEN_EXT_BYTE     1           //1-4

#define VAR_EXT_SECTOR 	     0
#define FIRMWARE_EXT_SECTOR  1
#define DEVICE_EXT_SECTOR 	 2
#define WIRELESS_EXT_SECTOR	 3
#define OTA_EXT_SECTOR	     10

#define DNS 0

#define USER_MQTT 0
#define VAKIO_MQTT 1

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

#define ON 1
#define OFF 0
#define MAXIMUM 100
#define DEFAUL_PULT_ID 1

#define MINUTE 60000
#define SECOND 1000

#define DEFAULT_AP_SSID "VakioCityAIR-350"
#define DEFAULT_AP_PASSWORD ""

#define DEFAULT_MQTT_PORT 1883

#define DEFAULT_USER_TOPIC "vakio"

#define VERSION "1.1.1"
#define SUBTYPE "default"
#define XTAL_FREQ "168"
#define SERIES "STM32"

#define TEN_TEMP_LIMIT 25

#define INFLOW_MAX_MODE_TIME (5 * MINUTE)

#define SPEED_ARR_DEFAUL               \
    {                                  \
        0, 20, 27, 41, 55, 70, 85, 100 \
    }
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
//////////////////////////////////////////////////
typedef enum
{
    INFLOW_MODE,
    SMART_MODE,
    INFLOW_MAX_MODE
} device_modes;

typedef struct
{
    bool find;
    bool error;
} remote_control_s;

typedef struct
{
    uint8_t deviation;   // отклонение должно быть 3
    uint8_t check_time;  // время проверки должно быть 1-5 минут
    uint8_t step_pwm;    // шаг шим должно быть 5
    uint16_t Kp;
    uint8_t Ki;
    uint8_t Kd;
} extra_options_s;

typedef enum
{
    REGULAR_MODE,
    SERVICE_MODE,
    DISTRIBUTION_MODE,
    RESET_ERROR_MODE,
    REBOOT_MODE,
    ACCEPT_MODE
} service_response_modes;

typedef struct
{
    float out;
    float in;
    bool out_state;
    bool in_state;
} sensors_data_s;

typedef struct
{
    char master_topic_data[50];
    bool status;
    bool set;
    char master_topic_system[52];
    char parameter[32];
    uint16_t master_id;
    uint16_t limit;
    uint8_t max_speed;
    int16_t value;
} smart_mode_s;

typedef struct
{
    bool damper;
    bool state;
    bool error_temp_hot;
    bool error_temp_cold;
    bool error_stop_hot;
    bool error_stop_cold;
    bool error_ds18b20;
    bool error_fan;
    remote_control_s remote_control;
    uint8_t mode;
    uint8_t last_mode;
    uint8_t inflow_speed;
    uint8_t smart_speed_pwm;
    smart_mode_s smart;
    uint8_t speed_arr[8];
    bool ten_state;
    uint8_t temp_limit;
    uint16_t ten_power;
    extra_options_s extra_options;
} device_s;

//////////////////////////////////

typedef struct
{
    char device_id[8];
    char user_id[8];
} vakio_user_s;

typedef struct
{
    char login[20];
    char password[20];
    char client_id[20];
    char host[20];
    uint16_t port;
    char topic[20];
} mqtt_client_s;

// Параметры для работы сети
typedef struct
{
    mqtt_client_s user_mqtt;    // Данные для подключения к пользовательскому брокеру
    mqtt_client_s vakio_mqtt;   // Данные для подключения к брокеру VAKIO
    vakio_user_s vakio;         // данные о пользователе
    bool mqtt_type;             // тип подключения к брокеру
    bool vakio_mqtt_connect_ip;
    char server_ip[20];         // для перепрошивки
    char domain[20];
} wireless_parameters_s;
//////////////////////////////////

uint8_t device_check_1_0_0;
uint32_t device_ota_len;
uint8_t device_firmware;

sensors_data_s *sensors_data;
wireless_parameters_s *wireless_params;
device_s *device;

device_s device_send;
device_s device_check;

bool iSendSpeed;
bool iSendMode;
bool iSendState;
bool iSendTemp;

bool mqtt_status;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SW_DIP3_Pin GPIO_PIN_2
#define SW_DIP3_GPIO_Port GPIOE
#define SW_DIP4_Pin GPIO_PIN_3
#define SW_DIP4_GPIO_Port GPIOE
#define SW_DIP5_Pin GPIO_PIN_4
#define SW_DIP5_GPIO_Port GPIOE
#define SW_DIP6_Pin GPIO_PIN_5
#define SW_DIP6_GPIO_Port GPIOE
#define SPI1_CS_Pin GPIO_PIN_4
#define SPI1_CS_GPIO_Port GPIOA
#define SERVICE_BTN_Pin GPIO_PIN_10
#define SERVICE_BTN_GPIO_Port GPIOD
#define RELAY_CH1_Pin GPIO_PIN_12
#define RELAY_CH1_GPIO_Port GPIOD
#define RELAY_CH2_Pin GPIO_PIN_13
#define RELAY_CH2_GPIO_Port GPIOD
#define RELAY_CH3_Pin GPIO_PIN_14
#define RELAY_CH3_GPIO_Port GPIOD
#define RELAY_CH4_Pin GPIO_PIN_15
#define RELAY_CH4_GPIO_Port GPIOD
#define USART1_DIR_Pin GPIO_PIN_12
#define USART1_DIR_GPIO_Port GPIOA
#define SSR_GATE1_Pin GPIO_PIN_10
#define SSR_GATE1_GPIO_Port GPIOC
#define UART5_DIR_Pin GPIO_PIN_3
#define UART5_DIR_GPIO_Port GPIOD
#define LED_LINK_Pin GPIO_PIN_5
#define LED_LINK_GPIO_Port GPIOD
#define LED_WORK_Pin GPIO_PIN_6
#define LED_WORK_GPIO_Port GPIOD
#define LED_FAILURE_Pin GPIO_PIN_7
#define LED_FAILURE_GPIO_Port GPIOD
#define SW_DIP1_Pin GPIO_PIN_0
#define SW_DIP1_GPIO_Port GPIOE
#define SW_DIP2_Pin GPIO_PIN_1
#define SW_DIP2_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
