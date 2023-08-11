#ifndef EPM_24C32_H_
#define EPM_24C32_H_

#include "main.h"

#if DEBUG_PRINT == 1
#define DEBUG_EPM(...) printf("EPM: "__VA_ARGS__);
#else
#define DEBUG_EPM(...)
#endif

#define EEPROM_ADDRESS (0x57 << 1) // АДРЕС УСТРОЙСТВА - 0x57
#define SIZEBLOCK 32 // размер страницы 32 байта

extern I2C_HandleTypeDef hi2c1;

void I2C_Scan(I2C_HandleTypeDef *hi2c);
void I2C_Error(char *er, uint32_t status);
void Write_mem(uint16_t addr, char *wstr, uint16_t len);
void Read_mem(uint16_t addr, char *rstr, uint16_t len);
//void Write_mem_single_block(uint16_t addr, char *wstr, uint16_t len);

#endif /* EPM_24C32_H_ */
