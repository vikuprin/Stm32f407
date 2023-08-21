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
#define DEBUG_PRINT 1
#if DEBUG_PRINT == 1
#define DEBUG_MAIN(...) printf("MAIN: "__VA_ARGS__);
#else
#define DEBUG_MAIN(...)
#endif

#define USER_MQTT 0
#define VAKIO_MQTT 1

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

#define ON 1
#define OFF 0
#define MAXIMUM 100
#define PULT_ID 1
#define FIRST_VALVE_ID 2

#define MINUTE 60000000
#define MIN_15_US 900000000
#define MIN_60_US 3600000000
#define MIN_15_OFF_AP 0
#define MIN_60_OFF_AP 1
#define MIN_NEVER_OFF_AP 2

#define DEFAULT_AP_SSID "VakioCityAIR"
#define DEFAULT_AP_PASSWORD ""

#define DEFAULT_MQTT_PORT 1883

#define DEFAULT_USER_TOPIC "vakio"

#define VERSION "1.1.2"
#define SUBTYPE "default"
#define XTAL_FREQ "40"
#define SERIES "esp32"

#define EXT_TEN_OFF_TEMP 45
#define EXT_TEN_OFF_UNDER 5
#define EXT_TEN_TEMP_LIMIT 22
#define EXT_TEN_TEMP_OVER 1
#define EXT_TEN_TEMP_UNDER 2

#define HEATER_OFF_TEMP 45
#define HEATER_OFF_UNDER 5
#define HEATER_TEMP_LIMIT 2
#define HEATER_TEMP_OVER 3
#define HEATER_TEMP_UNDER 3

#define TEN_OFF_TEMP 45
#define TEN_OFF_UNDER 5
#define TEN_TEMP_LIMIT 25
#define TEN_STARTED_VALUE 3
#define TEN_POWER_KOEF 3

#define INFLOW_MAX_MODE_TIME 300000000
#define MAX_BRG 250
#define SMART_CO2_DEL 200


#define MIN_FAN_POWER_TEN 30
#define SPEED_ARR_DEFAUL               \
    {                                  \
        0, 20, 27, 41, 55, 70, 85, 100 \
    }
#define RECUPERATOR_POWER_ARR          \
    {                                  \
        0, 0, 0, 60, 86, 115, 149, 180 \
    }
#define INFLOW_POWER_ARR                  \
    {                                     \
        0, 35, 55, 77, 109, 139, 170, 205 \
    }

#define DEFAULT_FILTER_ERROR_TIME 120000

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
//////////////////////////////////////////////////
typedef enum
{
    INFLOW_MODE,
    INFLOW_MAX_MODE,
    SMART_MODE
} device_modes;

typedef struct
{
    bool find;
    bool error;
} remote_control_s;

typedef struct
{
    uint8_t deviation;   // отклонение должно быть 3
    uint32_t check_time; // время проверки должно быть 5 минут
    uint8_t step_pwm;    // шаг шим должно быть 5
} extra_options_s;

typedef struct
{
    char out[32];
    char in[32];
} sensors_addr_s;

typedef struct
{
    float out;
    float out_real;
    float in;
    float max;
    float min;
    float delta_max;
    float delta_min;
    bool out_state;
    bool in_state;
} sensors_data_s;

typedef struct
{
	bool firmware_flag;
    bool damper;
    bool state;
    bool error_temp_hot;
    bool error_temp_cold;
    bool error_stop_hot;
    bool error_stop_cold;
    bool error_ds18_bus;  // не исправна шина DS18B20
    bool error_ds18_lack; // не исправен один из датчиков DS18B20
    remote_control_s remote_control;
	uint8_t check_1_0_0;
    uint8_t mode;
    uint8_t last_mode;
    uint8_t inflow_speed;
    uint8_t smart_speed_pwm;
    uint8_t speed_arr[8];
    sensors_addr_s sensors_addr;
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
    char login[12];
    char password[12];
    char client_id[12];
    char host[8];
    int port;
    char topic[12];
} mqtt_client_s;

// Параметры для работы сети
typedef struct
{
    mqtt_client_s user_mqtt;    // Данные для подключения к пользовательскому брокеру
    mqtt_client_s vakio_mqtt;   // Данные для подключения к брокеру VAKIO
    vakio_user_s vakio;         // данные о пользователе
    uint8_t soft_ap_off_time; // Через сколько выключиться точка доступа 0-2 (есть массив)
    bool mqtt_type;             // тип подключения к брокеру
    bool vakio_mqtt_connect_ip;
    char server_ip[8];         // для перепрошивки
    char domain[12];
} wireless_parameters_s;

////////////////////////////////

typedef struct
{
    bool state_log;
    bool state;
    int temp_limit;
    int off_temp;
    int off_temp_under;
    uint16_t power;
    uint8_t started_value;
    int power_koef;
} ten_s;

typedef struct
{
    ten_s ten;
    int koef;
    float delta;
    float delta_high;
    int koef_grow;
    uint64_t update_koef_time;
    bool on_off_ten;
} heaters_s;

//////////////////////////////////

uint16_t co2_master_data[15];

sensors_data_s *sensors_data;
wireless_parameters_s *wireless_params;
device_s *device;
heaters_s *heaters;
uint8_t inst_speed;
device_s device_send;
device_s device_check;
heaters_s heaters_send;
heaters_s heaters_check;

bool iSendSpeed;
bool iSendMode;
bool iSendState;
bool iSendTemp;

bool netif_link; // статус сети
bool mqtt_status;  // статус mqtt
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
#define HEAT_VALVE_GATE1_Pin GPIO_PIN_3
#define HEAT_VALVE_GATE1_GPIO_Port GPIOA
#define SPI1_CS_Pin GPIO_PIN_4
#define SPI1_CS_GPIO_Port GPIOA
#define FAN_FG1_Pin GPIO_PIN_0
#define FAN_FG1_GPIO_Port GPIOB
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
