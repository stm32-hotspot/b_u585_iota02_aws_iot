/**
  ******************************************************************************
  * @file    stsafea_crc.h
  * @brief   STM32_Nucleo CRC
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STSAFEA1_CRC_H
#define STSAFEA1_CRC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "safea1_conf.h"
/** @addtogroup CRC
  * @{
  */


/** @addtogroup STSAFEA1
  * @{
  */


/** @addtogroup STSAFEA1_CRC
  * @{
  */


/** @defgroup STSAFEA1_CRC_Exported_Constants STSAFEA1 CRC Exported Constants
  * @{
  */
/**
  * @}STSAFEA1_CRC_Exported_Constants
  */


/** @defgroup STSAFEA1_CRC_Private_Types STSAFEA1 CRC Private types
  * @{
  */


/**
  * @}STSAFEA1_CRC_Private_Types
  */


/** @defgroup STSAFEA1_LOW_LEVEL_Exported_Variables LOW LEVEL Exported Constants
  * @{
  */


/**
  * @}STSAFEA1_LOW_LEVEL_Exported_Variables
  */


/** @addtogroup STSAFEA1_CRC_Exported_Functions
  * @{
  */
int32_t CRC16X25_Init(void);
uint32_t CRC_Compute(uint8_t *pData1, uint16_t Length1, uint8_t *pData2, uint16_t Length2);
/**
  * @}STSAFEA1_CRC_Exported_Functions
  */


/**
  * @}STSAFEA1_CRC
  */

/**
  * @}STSAFEA1
  */


/**
  * @}CRC
  */


#ifdef __cplusplus
}
#endif

#endif /* STSAFEA1_CRC_H */
