#ifndef __MODBUS_CONFIG_H
#define __MODBUS_CONFIG_H

#include "main.h"

typedef struct
{
	uint8_t 		slave_addr;
	uint8_t         function;
	uint16_t 		reg_addr;
	uint16_t 		data;
} modbus_value;

modbus_value Holding_register;

int32_t modbus_get_holding(uint8_t slave_id, uint16_t reg_start);
int8_t modbus_set_holding(uint8_t slave_id, uint16_t reg_start, uint16_t data);

void modbus_rx_handler();
void modbus_tx_handler();

#endif /*__MODBUS_CONFIG_H */
