#include "main.h"
#ifndef THIRD_PARTY_MODBUS_LIB_CONFIG_MODBUSCONFIG_H_
#define THIRD_PARTY_MODBUS_LIB_CONFIG_MODBUSCONFIG_H_



/* Uncomment the following line to enable support for Modbus RTU over USB CDC profile. Only tested for BluePill f103 board. */
//#define ENABLE_USB_CDC 1

/* Uncomment the following line to enable support for Modbus TCP. Only tested for Nucleo144-F429ZI. */
//#define ENABLE_TCP 1

/* Uncomment the following line to enable support for Modbus RTU USART DMA mode. Only tested for Nucleo144-F429ZI.  */


#define T35  2              // Timer T35 period (in ticks) for end frame detection.
#define MAX_BUFFER  128	    // Maximum size for the communication buffer in bytes.
#define TIMEOUT_MODBUS 1000 // Timeout for master query (in ticks)
#define MAX_M_HANDLERS 2    //Maximum number of modbus handlers that can work concurrently
#define MAX_TELEGRAMS 2     //Max number of Telegrams in master queue




#endif /* THIRD_PARTY_MODBUS_LIB_CONFIG_MODBUSCONFIG_H_ */

int8_t modbus_master_set_one(uint8_t slave_id, uint8_t function_code, uint16_t reg_start, uint16_t data);
int32_t modbus_master_get_one(uint8_t slave_id, uint8_t function_code, uint16_t reg_start);
int8_t modbus_master_set(uint8_t slave_id, uint8_t function_code, uint16_t reg_start, uint8_t reg_size);
int8_t modbus_master_get(uint8_t slave_id, uint8_t function_code, uint16_t reg_start, uint8_t reg_size);
void init_modbus_master();
void ModbusMasterTask(void *argument);
