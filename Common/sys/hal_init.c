/*
 * FreeRTOS STM32 Reference Integration
 *
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#include "FreeRTOS.h"
#include "main.h"
#include "hw_defs.h"
#include "task.h"
#include "logging.h"
#include <string.h>

/* Global peripheral handles */

/* local function prototypes */

void hw_init(void)
{
  uint32_t ulCsrFlags = 0;

  ulCsrFlags = RCC->CSR;

  /* To make the compiler happy */
  ulCsrFlags = ulCsrFlags;

  __HAL_RCC_CLEAR_RESET_FLAGS();
}

typedef void (*VectorTable_t)(void);

#define NUM_USER_IRQ               ( FMAC_IRQn + 1 )      /* MCU specific */
#define VECTOR_TABLE_SIZE          ( NVIC_USER_IRQ_OFFSET + NUM_USER_IRQ )
#define VECTOR_TABLE_ALIGN_CM33    0x400U

static VectorTable_t pulVectorTableSRAM[VECTOR_TABLE_SIZE] __attribute__( ( aligned( VECTOR_TABLE_ALIGN_CM33 ) ) );

/* Relocate vector table to ram for runtime interrupt registration */
void vRelocateVectorTable(void)
{
  /* Disable interrupts */
  __disable_irq();

#if defined (HAL_DCACHE_MODULE_ENABLED)
  HAL_ICACHE_Disable();
#endif

#if defined (HAL_ICACHE_MODULE_ENABLED)
  HAL_DCACHE_Disable(&hdcache1);
#endif

  /* Copy vector table to ram */
  (void) memcpy(pulVectorTableSRAM, (uint32_t*) SCB->VTOR, sizeof(uint32_t) * VECTOR_TABLE_SIZE);

  SCB->VTOR = (uint32_t) pulVectorTableSRAM;

  __DSB();
  __ISB();

#if defined (HAL_DCACHE_MODULE_ENABLED)
  HAL_DCACHE_Invalidate(&hdcache1);
#endif

#if defined (HAL_ICACHE_MODULE_ENABLED)
  HAL_ICACHE_Invalidate();
  HAL_ICACHE_Enable();
#endif

#if defined (HAL_DCACHE_MODULE_ENABLED)
  HAL_DCACHE_Enable(&hdcache1);
#endif

  __enable_irq();
}
