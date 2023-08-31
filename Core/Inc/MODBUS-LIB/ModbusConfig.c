#include "ModbusConfig.h"
#include "Modbus.h"

modbusHandler_t ModbusH2;
int16_t ModbusDATA2[8];
modbus_t telegram;

int8_t modbus_master_set_one(uint8_t slave_id, uint8_t function_code, uint16_t reg_start, uint16_t data) // modbus_parameter: 0-HOLDING, 1-INPUT, 2-COIL, 3-DISCRETE
{
	uint32_t u32NotificationValue;

	telegram.u8id = slave_id; // slave address
	telegram.u8fct = function_code; // function code (this one is registers read)
	telegram.u16RegAdd = reg_start; // start address in slave
	telegram.u16CoilsNo = 1; // number of elements (coils or registers) to read
	ModbusDATA2[0] = data;
	telegram.u16reg = ModbusDATA2; // pointer to a memory array

	ModbusQuery(&ModbusH2, telegram);
	u32NotificationValue = ulTaskNotifyTake(pdTRUE, 500);
	if (u32NotificationValue == 0)
	{
		return -1;
	}
	if (ModbusH2.i8lastError == ERR_TIME_OUT)
	{
		return -2;
	}
	return 1;
}

int32_t modbus_master_get_one(uint8_t slave_id, uint8_t function_code, uint16_t reg_start) // modbus_parameter: 0-HOLDING, 1-INPUT, 2-COIL, 3-DISCRETE
{
	uint32_t u32NotificationValue;

	telegram.u8id = slave_id; // slave address
	telegram.u8fct = function_code; // function code (this one is registers read)
	telegram.u16RegAdd = reg_start; // start address in slave
	telegram.u16CoilsNo = 1; // number of elements (coils or registers) to read
	telegram.u16reg = ModbusDATA2; // pointer to a memory array

	ModbusQuery(&ModbusH2, telegram);
	u32NotificationValue = ulTaskNotifyTake(pdTRUE, 500);
	if (u32NotificationValue == 0)
	{
		return -1;
	}
	if (ModbusH2.i8lastError == ERR_TIME_OUT)
	{
		return -2;
	}
	return ModbusDATA2[0];
}

int8_t modbus_master_set(uint8_t slave_id, uint8_t function_code, uint16_t reg_start, uint8_t reg_size) // modbus_parameter: 0-HOLDING, 1-INPUT, 2-COIL, 3-DISCRETE
{
	uint32_t u32NotificationValue;

	telegram.u8id = slave_id; // slave address
	telegram.u8fct = function_code; // function code (this one is registers read)
	telegram.u16RegAdd = reg_start; // start address in slave
	telegram.u16CoilsNo = reg_size; // number of elements (coils or registers) to read
	telegram.u16reg = ModbusDATA2; // pointer to a memory array

	ModbusQuery(&ModbusH2, telegram);
	u32NotificationValue = ulTaskNotifyTake(pdTRUE, 500);
	if (u32NotificationValue == 0)
	{
		return -1;
	}
	if (ModbusH2.i8lastError == ERR_TIME_OUT)
	{
		return -2;
	}
	return 1;
}

int8_t modbus_master_get(uint8_t slave_id, uint8_t function_code, uint16_t reg_start, uint8_t reg_size) // modbus_parameter: 0-HOLDING, 1-INPUT, 2-COIL, 3-DISCRETE
{
	uint32_t u32NotificationValue;

	telegram.u8id = slave_id; // slave address
	telegram.u8fct = function_code; // function code (this one is registers read)
	telegram.u16RegAdd = reg_start; // start address in slave
	telegram.u16CoilsNo = reg_size; // number of elements (coils or registers) to read
	telegram.u16reg = ModbusDATA2; // pointer to a memory array

	ModbusQuery(&ModbusH2, telegram);
	u32NotificationValue = ulTaskNotifyTake(pdTRUE, 500);
	if (u32NotificationValue == 0)
	{
		return -1;
	}
	if (ModbusH2.i8lastError == ERR_TIME_OUT)
	{
		return -2;
	}
	return 1;
}

/* Master initialization */
void init_modbus_master()
{
	ModbusH2.uModbusType = MB_MASTER;
	ModbusH2.port =  &huart5;
	ModbusH2.u8id = 0; // For master it must be 0
	ModbusH2.u16timeOut = 100;
	ModbusH2.EN_Port = UART5_DIR_GPIO_Port;
	ModbusH2.EN_Pin = UART5_DIR_Pin;
	ModbusH2.u16regs = ModbusDATA2;
	ModbusH2.u16regsize= sizeof(ModbusDATA2)/sizeof(ModbusDATA2[0]);
	ModbusH2.xTypeHW = USART_HW_DMA;
	//Initialize Modbus library
	ModbusInit(&ModbusH2);
	//Start capturing traffic on serial Port
	ModbusStart(&ModbusH2);
}
/***********/
/* Slave initialization */
//void init_modbus_slave()
//{
//    ModbusH.uModbusType = MB_SLAVE;
//    ModbusH.port =  &huart3;
//    ModbusH.u8id = 1; // For master it must be 0
//    ModbusH.u16timeOut = 1000;
//    ModbusH.EN_Port = NULL;
//    ModbusH.EN_Pin = 0;
//    ModbusH.u16regs = ModbusDATA;
//    ModbusH.u16regsize= sizeof(ModbusDATA)/sizeof(ModbusDATA[0]);
//    ModbusH.xTypeHW = USART_HW_DMA;
//    //Initialize Modbus library
//    ModbusInit(&ModbusH);
//    //Start capturing traffic on serial Port
//    ModbusStart(&ModbusH);
//}
/***********/
