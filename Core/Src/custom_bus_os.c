/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : custom_bus.c
  * @brief          : source file for the BSP BUS IO driver
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
*/
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "semphr.h"
#include "custom_bus.h"

static SemaphoreHandle_t I2C2_MutexHandle;

/**
  * @}
  */

/** @defgroup CUSTOM_LOW_LEVEL_Private_Functions CUSTOM LOW LEVEL Private Functions
  * @{
  */

/** @defgroup CUSTOM_BUS_Exported_Functions CUSTOM_BUS Exported Functions
  * @{
  */

/* BUS IO driver over I2C Peripheral */
/*******************************************************************************
                            BUS OPERATIONS OVER I2C
*******************************************************************************/
/**
  * @brief  Initialize I2C HAL
  * @retval BSP status
  */
int32_t BSP_I2C2_Init_OS(void)
{
  int32_t ret = BSP_ERROR_NONE;

  taskENTER_CRITICAL();
  if (I2C2_MutexHandle == NULL)
  {
    I2C2_MutexHandle = xSemaphoreCreateMutex();
  }
  taskEXIT_CRITICAL();

  xSemaphoreTake(I2C2_MutexHandle, portMAX_DELAY);
  ret = BSP_I2C2_Init();
  xSemaphoreGive(I2C2_MutexHandle);

  return ret;
}

/**
  * @brief  DeInitialize I2C HAL.
  * @retval BSP status
  */
int32_t BSP_I2C2_DeInit_OS(void)
{
  int32_t ret = BSP_ERROR_NONE;

  xSemaphoreTake(I2C2_MutexHandle, portMAX_DELAY);
  ret = BSP_I2C2_DeInit();
  xSemaphoreGive(I2C2_MutexHandle);

  return ret;
}

/**
  * @brief  Check whether the I2C bus is ready.
  * @param DevAddr : I2C device address
  * @param Trials : Check trials number
  * @retval BSP status
  */
int32_t BSP_I2C2_IsReady_OS(uint16_t DevAddr, uint32_t Trials)
{
  int32_t ret = BSP_ERROR_NONE;

  xSemaphoreTake(I2C2_MutexHandle, portMAX_DELAY);
  ret = BSP_I2C2_IsReady(DevAddr, Trials);
  xSemaphoreGive(I2C2_MutexHandle);

  return ret;
}

/**
  * @brief  Write a value in a register of the device through BUS.
  * @param  DevAddr Device address on Bus.
  * @param  Reg    The target register address to write
  * @param  pData  Pointer to data buffer to write
  * @param  Length Data Length
  * @retval BSP status
  */

int32_t BSP_I2C2_WriteReg_OS(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  xSemaphoreTake(I2C2_MutexHandle, portMAX_DELAY);
  ret = BSP_I2C2_WriteReg(DevAddr, Reg, pData, Length);
  xSemaphoreGive(I2C2_MutexHandle);

  return ret;
}

/**
  * @brief  Read a register of the device through BUS
  * @param  DevAddr Device address on Bus.
  * @param  Reg    The target register address to read
  * @param  pData  Pointer to data buffer to read
  * @param  Length Data Length
  * @retval BSP status
  */
int32_t  BSP_I2C2_ReadReg_OS(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  xSemaphoreTake(I2C2_MutexHandle, portMAX_DELAY);
  ret = BSP_I2C2_ReadReg(DevAddr, Reg, pData, Length);
  xSemaphoreGive(I2C2_MutexHandle);

  return ret;
}

/**

  * @brief  Write a value in a register of the device through BUS.
  * @param  DevAddr Device address on Bus.
  * @param  Reg    The target register address to write

  * @param  pData  Pointer to data buffer to write
  * @param  Length Data Length
  * @retval BSP statu
  */
int32_t BSP_I2C2_WriteReg16_OS(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  xSemaphoreTake(I2C2_MutexHandle, portMAX_DELAY);
  ret = BSP_I2C2_WriteReg16(DevAddr, Reg, pData, Length);
  xSemaphoreGive(I2C2_MutexHandle);

  return ret;
}

/**
  * @brief  Read registers through a bus (16 bits)
  * @param  DevAddr: Device address on BUS
  * @param  Reg: The target register address to read
  * @param  Length Data Length
  * @retval BSP status
  */
int32_t  BSP_I2C2_ReadReg16_OS(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  xSemaphoreTake(I2C2_MutexHandle, portMAX_DELAY);
  ret = BSP_I2C2_ReadReg16(DevAddr, Reg, pData, Length);
  xSemaphoreGive(I2C2_MutexHandle);

  return ret;
}

/**
  * @brief  Send an amount width data through bus (Simplex)
  * @param  DevAddr: Device address on Bus.
  * @param  pData: Data pointer
  * @param  Length: Data length
  * @retval BSP status
  */
int32_t BSP_I2C2_Send_OS(uint16_t DevAddr, uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  xSemaphoreTake(I2C2_MutexHandle, portMAX_DELAY);
  ret = BSP_I2C2_Send(DevAddr, pData, Length);
  xSemaphoreGive(I2C2_MutexHandle);

  return ret;
}

/**
  * @brief  Receive an amount of data through a bus (Simplex)
  * @param  DevAddr: Device address on Bus.
  * @param  pData: Data pointer
  * @param  Length: Data length
  * @retval BSP status
  */
int32_t BSP_I2C2_Recv_OS(uint16_t DevAddr, uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_NONE;

  xSemaphoreTake(I2C2_MutexHandle, portMAX_DELAY);
  ret = BSP_I2C2_Recv(DevAddr, pData, Length);
  xSemaphoreGive(I2C2_MutexHandle);

  return ret;
}

/**
  * @brief  Return system tick in ms
  * @retval Current HAL time base time stamp
  */
int32_t BSP_GetTick_OS(void)
{
  return HAL_GetTick();
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

