#include "main.h"
#include "flash_interface.h"
#include <stdbool.h>
#include "logging_levels.h"
#define LOG_LEVEL    LOG_NONE

#include "FreeRTOS.h"
#include "task.h"

HAL_StatusTypeDef FLASH_Write(uint32_t address, uint32_t * data, uint32_t buffer_size)
{
	bool status = HAL_OK;
#if defined(HAL_ICACHE_MODULE_ENABLED)
  HAL_ICACHE_Disable();
#endif

  HAL_FLASH_Unlock();

  for (int i = 0; i < buffer_size; i += FLASH_ADDRESS_INC_SIZE)
  {
	  LogInfo("Writing to address : 0x%08X\r\n", (int)(i + address));

    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD, address + i, (uint32_t) data + i) != HAL_OK)
    {
      status = HAL_ERROR;
    }
  }

  HAL_FLASH_Lock();

#if defined(HAL_ICACHE_MODULE_ENABLED)
  HAL_ICACHE_Enable();
#endif

  return status;
}

HAL_StatusTypeDef FLASH_Erase(uint32_t address)
{
  FLASH_EraseInitTypeDef EraseInitStruct;
  uint32_t SectorError = 0;
  uint32_t status = HAL_OK;

#if defined(HAL_ICACHE_MODULE_ENABLED)
  HAL_ICACHE_Disable();
#endif

  HAL_FLASH_Unlock();

  EraseInitStruct.Page      = (address - FLASH_START_ADDRESS)/FLASH_PAGE_SIZE;
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.NbPages   = 1;

  if (EraseInitStruct.Page <= 127U)
  {
	  EraseInitStruct.Banks = FLASH_BANK_1;
  }
  else if (EraseInitStruct.Page <= 255U)
  {
	  EraseInitStruct.Banks = FLASH_BANK_2;
  }
  else
  {
    return HAL_ERROR;
  }

  LogInfo("Erasing page number %d\r\n", (int)EraseInitStruct.Page);

  if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
  {
    status = HAL_ERROR;
  }

  HAL_FLASH_Lock();

#if defined(HAL_ICACHE_MODULE_ENABLED)
  HAL_ICACHE_Enable();
#endif

  return status;
}


HAL_StatusTypeDef isFlashEmpty(uint32_t startAddress, uint32_t size) {
    // Read the flash memory
    uint32_t flash_data;
    for (uint32_t i = 0; i < size; i += 4) {
        flash_data = *(__IO uint32_t*)(startAddress + i);
        if (flash_data != 0xFFFFFFFF) {
            // Flash memory section is not empty
            return HAL_ERROR;
        }
    }

    // Flash memory section is empty
    return HAL_OK;
}

