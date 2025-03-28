/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    hardware_rng.c
 * @author  GPM Application Team
 * @version V1.2.1
 * @date    14-April-2017
 * @brief   mbedtls alternate entropy data function.
 *          the mbedtls_hardware_poll() is customized to use the STM32 RNG
 *          to generate random data, required for TLS encryption algorithms.
 *
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
/* USER CODE END Header */
#include MBEDTLS_CONFIG_FILE

#include "string.h"
#include "entropy_poll.h"

#if defined( MBEDTLS_ENTROPY_HARDWARE_ALT )

extern UBaseType_t uxRand(void);

int mbedtls_hardware_poll( void *Data, unsigned char *Output, size_t Len, size_t *oLen )
{
  uint32_t index;
  uint32_t randomValue;

  for (index = 0; index < Len/4; index++)
  {
    randomValue = uxRand();

    *oLen += 4;
    memset(&(Output[index * 4]), (int)randomValue, 4);
  }

  return 0;
}

#endif
