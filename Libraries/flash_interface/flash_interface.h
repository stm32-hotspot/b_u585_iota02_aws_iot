#include "main.h"

#define FLASH_ADDRESS_INC_SIZE       (16)
#define FLASH_START_ADDRESS          (0x08000000)

HAL_StatusTypeDef FLASH_Write(uint32_t address, uint32_t * data, uint32_t buffer_size);
HAL_StatusTypeDef FLASH_Erase(uint32_t address);
