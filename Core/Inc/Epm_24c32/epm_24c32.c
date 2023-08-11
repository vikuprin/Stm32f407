#include <stdio.h>
#include "epm_24c32.h"

void Write_mem(uint16_t addr, char *wstr, uint16_t len)
{
	uint16_t cycle = len / SIZEBLOCK;
	uint32_t status = 0;

	for(uint16_t i = 0; i < cycle + 1; i++)
	{
		if(len > SIZEBLOCK)
		{
			status = HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDRESS, addr, I2C_MEMADD_SIZE_16BIT, (uint8_t*)wstr, SIZEBLOCK, HAL_MAX_DELAY);
			wstr = wstr + SIZEBLOCK;
			len = len - SIZEBLOCK;
			addr = addr + SIZEBLOCK;
		}
		else
		{
			status = HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDRESS, addr, I2C_MEMADD_SIZE_16BIT, (uint8_t*)wstr, len, HAL_MAX_DELAY);
		}

		if(status != HAL_OK)
		{
			char str[32] = {0,};
			snprintf(str, 32, "HAL_I2C_Mem_Write");
			I2C_Error(str, status);
		}

		for(uint8_t i = 0; i < 100; i++) // ждём пока eeprom доделает свои делишки и ответит что готова.
		{
			status = HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_ADDRESS, 1, 100);

			if(status == HAL_OK) break;

			if(i > 99)
			{
				char str[32] = {0,};
				snprintf(str, 32, "HAL_I2C_IsDeviceReady");
				I2C_Error(str, status);
			}
		}
	}
}

void Read_mem(uint16_t addr, char *rstr, uint16_t len)
{
	uint32_t status = HAL_I2C_Mem_Read(&hi2c1, EEPROM_ADDRESS, addr, I2C_MEMADD_SIZE_16BIT, (uint8_t*)rstr, len, HAL_MAX_DELAY);

	if(status != HAL_OK)
	{
		char str[32] = {0,};
		snprintf(str, 32, "HAL_I2C_Mem_Read");
		I2C_Error(str, status);
	}
}

void I2C_Scan(I2C_HandleTypeDef *hi2c) // сканирует шину и показывает адреса устройств
{
	DEBUG_EPM("Scanning I2C bus...\n");
    for(uint16_t i = 0; i < 128; i++)
    {
        if(HAL_I2C_IsDeviceReady(hi2c, i << 1, 1, 100) == HAL_OK)
        {
        	char msg[64] = {0,};
            snprintf(msg, 64, "Device: 0x%02X\n", i);
            DEBUG_EPM("%с", msg);
            //HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 1000);
        }
        else
        {
        	DEBUG_EPM("No device I2C = %i\n", i);
        }
    }
}

void I2C_Error(char *er, uint32_t status) // ошибки i2c
{
	char str[64] = {0,};

	switch(status)
	{
		case HAL_ERROR:
			snprintf(str, 64, "%s - HAL_ERROR\n", er);
			//HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), 1000);
		break;

		case HAL_BUSY:
			snprintf(str, 64, "%s - HAL_BUSY\n", er);
			//HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), 1000);
		break;

		case HAL_TIMEOUT:
			snprintf(str, 64, "%s - HAL_TIMEOUT\n", er);
			//HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), 1000);
		break;

		default:
		break;
	}

	uint32_t err = HAL_I2C_GetError(&hi2c1);

	switch(err)
	{
		case HAL_I2C_ERROR_NONE:
			snprintf(str, 64, "HAL_I2C_ERROR_NONE\n");
			//HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), 1000);
		break;

		case HAL_I2C_ERROR_BERR:
			snprintf(str, 64, "HAL_I2C_ERROR_BERR\n");
			//HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), 1000);
		break;

		case HAL_I2C_ERROR_ARLO:
			snprintf(str, 64, "HAL_I2C_ERROR_ARLO\n");
			//HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), 1000);
		break;

		case HAL_I2C_ERROR_AF:
			snprintf(str, 64, "HAL_I2C_ERROR_AF\n");
			//HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), 1000);
		break;

		case HAL_I2C_ERROR_OVR:
			snprintf(str, 64, "HAL_I2C_ERROR_OVR\n");
			//HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), 1000);
		break;

		case HAL_I2C_ERROR_DMA:
			snprintf(str, 64, "HAL_I2C_ERROR_DMA\n");
			//HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), 1000);
		break;

		case HAL_I2C_ERROR_TIMEOUT:
			snprintf(str, 64, "HAL_I2C_ERROR_TIMEOUT\n");
			//HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), 1000);
		break;

		default:
		break;
	}

	while(1){}; // после вывода ошибки программа зацикливается
}

/*void Write_mem_single_block(uint16_t addr, char *wstr, uint16_t len) // записывает в eeprom один блок (32 байта), перед чтением нужна пауза
{
	uint16_t status = HAL_I2C_Mem_Write(&hi2c1, EEPROM_ADDRESS, addr, I2C_MEMADD_SIZE_16BIT, (uint8_t*)wstr, len, HAL_MAX_DELAY);

	if(status != HAL_OK)
	{
		char str[32] = {0,};
		snprintf(str, 32, "HAL_I2C_Mem_Read");
		I2C_Error(str, status);
	}
}*/


