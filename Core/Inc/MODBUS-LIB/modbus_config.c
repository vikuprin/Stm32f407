#include "modbus_config.h"
#include "modbus_crc.h"
#include "cmsis_os.h"

bool modbus_transaction;
uint8_t RxData[8];
uint8_t TxData[8];

uint16_t Data[10];

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	HAL_GPIO_WritePin(UART5_DIR_GPIO_Port, UART5_DIR_Pin , GPIO_PIN_RESET);
	HAL_UARTEx_ReceiveToIdle_IT(&huart5, RxData, 8);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(RxData[0] == Holding_register.slave_addr && RxData[1] == Holding_register.function)
	{
		modbus_transaction = true;
		Data[0] = RxData[3]<<8 | RxData[4];
//		Data[1] = RxData[5]<<8 | RxData[6];
//		Data[2] = RxData[7]<<8 | RxData[8];
//		Data[3] = RxData[9]<<8 | RxData[10];
	//	Data[4] = RxData[11]<<8 | RxData[12];
	}
}

void sendData (uint8_t *data)
{
	HAL_GPIO_WritePin(UART5_DIR_GPIO_Port, UART5_DIR_Pin, GPIO_PIN_SET);
	HAL_UART_Transmit_DMA(&huart5, data, 8);
}



int8_t modbus_set_holding(uint8_t slave_id, uint16_t reg_start, uint16_t data)
{
	TxData[0] = slave_id;  // slave address
    TxData[1] = 0x06;  // Function code for Read Input Registers
    TxData[2] = 0;
    TxData[3] = reg_start;
    TxData[4] = 0;
    TxData[5] = data;
    uint16_t crc = crc16(TxData, 6);
    TxData[6] = crc&0xFF;   // CRC LOW
    TxData[7] = (crc>>8)&0xFF;  // CRC HIGH

    Holding_register.slave_addr = slave_id;
    Holding_register.function = 0x06;
    Holding_register.reg_addr = reg_start;
    Holding_register.data = data;
    sendData(TxData);
    osDelay(100);
    if(modbus_transaction)
    {
    	modbus_transaction = false;
    	return 1;
    }
	return -1;
}

int32_t modbus_get_holding(uint8_t slave_id, uint16_t reg_start)
{
	TxData[0] = slave_id;  // slave address
    TxData[1] = 0x03;  // Function code for Read Input Registers
    TxData[2] = 0;
    TxData[3] = reg_start;
    TxData[4] = 0;
    TxData[5] = 0x01;
    uint16_t crc = crc16(TxData, 6);
    TxData[6] = crc&0xFF;   // CRC LOW
    TxData[7] = (crc>>8)&0xFF;  // CRC HIGH

    Holding_register.slave_addr = slave_id;
    Holding_register.function = 0x03;
    Holding_register.reg_addr = reg_start;

    sendData(TxData);
    osDelay(100);
    if(modbus_transaction)
    {
    	modbus_transaction = false;
    	return Data[0];
    }
	return -1;
}

