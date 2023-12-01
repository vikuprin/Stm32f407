#include "bootloader.h"
#include "w25qxx.h"
#include "storage.h"
#include "FLASH_SECTOR_F4.h"
#include "spi.h"

extern FLASH_ProcessTypeDef pFlash;

uint32_t address_flash = OTA_ADDR_FLASH_1;
static uint8_t buff_ota[256];

int flash_data(char* buf, int len)
{
  __HAL_FLASH_PREFETCH_BUFFER_DISABLE();
  __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_5);

  HAL_StatusTypeDef ret;

  ret = HAL_FLASH_Unlock();
  if (ret != HAL_OK)
	  return ret;

  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP
                         | FLASH_FLAG_OPERR
                         | FLASH_FLAG_WRPERR
                         | FLASH_FLAG_PGAERR
                         | FLASH_FLAG_PGSERR);

  if (pFlash.ErrorCode != 0)
      return pFlash.ErrorCode;

  for (int i = 0; i < len; i++)
  {
    FLASH_WaitForLastOperation(HAL_MAX_DELAY);
    ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, address_flash, buf[i]);
    if (ret != HAL_OK)
    {
    	printf("App Flash Write Error\n");
        break;
    }
    address_flash++;
  }

  FLASH_WaitForLastOperation(500);

  ret = HAL_FLASH_Lock();
  if(ret != HAL_OK)
	  return ret;

  return ret;
}

static void jumpToApp(uint32_t start_program_addr)
{
	HAL_DeInit();
	__set_MSP(*((volatile uint32_t *) (start_program_addr)));
	__DMB();
	SCB->VTOR = start_program_addr;
	__DSB();
    uint32_t JumpAddress = *((volatile uint32_t*) (start_program_addr + 4));
    void (*reset_handler) (void) = (void*) JumpAddress;
    reset_handler();
}

void bootloader()
{
	MX_SPI1_Init();
	W25qxx_Init();
	read_ota_byte();
	if (device_ota_len > (128 * 1024))
	{
		Flash_Delete_Data(OTA_ADDR_FLASH_1);
		Flash_Delete_Data(OTA_ADDR_FLASH_2);
		Flash_Delete_Data(OTA_ADDR_FLASH_3);
		Flash_Delete_Data(OTA_ADDR_FLASH_4);

		uint16_t start_page_addr = OTA_EXT_BYTE / 256;
		uint16_t num_page_addr = device_ota_len / 256 + 1;

		for (uint16_t i = 0; i < num_page_addr; i++)
		{
			W25qxx_ReadPage(buff_ota, start_page_addr, 0, 256);
			Flash_Write_Data(address_flash, buff_ota, 256);
			start_page_addr++;
		}
		jumpToApp(OTA_ADDR_FLASH_1);
	}
}
