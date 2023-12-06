#include "bootloader.h"
#include "w25qxx.h"
#include "storage.h"
#include "FLASH_SECTOR_F4.h"
#include "ota.h"

extern FLASH_ProcessTypeDef pFlash;

void bootloader()
{
	W25qxx_Init();
	read_ota_byte();
	W25qxx_ReadSector(device_firmware, FIRMWARE_EXT_SECTOR, 0, 1);

    uint32_t *data1 = (uint32_t*)OTA_ADDR_FLASH_1;
    uint32_t *data2 = (uint32_t*)OTA_ADDR_FLASH_2;
    uint32_t *data3 = (uint32_t*)OTA_ADDR_FLASH_3;
    uint32_t hasSW = data1[0];
	if (hasSW != 0xFFFFFFFF && device_ota_len > (128 * 1024))
	{
		Flash_Write_Data(MAIN_ADDR_FLASH_1, data1, 32768);
		Flash_Write_Data(MAIN_ADDR_FLASH_2, data2, 32768);
		if (device_ota_len > (256 * 1024))
			Flash_Write_Data(MAIN_ADDR_FLASH_3, data3, 32768);

		W25qxx_EraseSector(VAR_EXT_SECTOR);
		device_firmware = 1;
		W25qxx_WriteSector(device_firmware, FIRMWARE_EXT_SECTOR, 0, 1);

		jumpToApp(MAIN_ADDR_FLASH_1);
	}
	else if (device_firmware == 1)
	{
		jumpToApp(MAIN_ADDR_FLASH_1);
	}
}
