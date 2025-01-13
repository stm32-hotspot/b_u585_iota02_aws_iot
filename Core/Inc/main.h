/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"

#include "safea1_conf.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void MX_OCTOSPI2_Init(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RunTimeStats_Timer htim5
#define MXCHIP_NOTIFY_EXTI_IRQn EXTI14_IRQn
#define xConsoleHandle huart1
#define MXCHIP_SPI hspi2
#define USE_SENSORS 1
#define MXCHIP_FLOW_EXTI_IRQn EXTI15_IRQn
#define MX25LM_OSPI hospi2
#define MXCHIP_FLOW_Pin GPIO_PIN_15
#define MXCHIP_FLOW_GPIO_Port GPIOG
#define MXCHIP_FLOW_EXTI_IRQn EXTI15_IRQn
#define PH3_BOOT0_Pin GPIO_PIN_3
#define PH3_BOOT0_GPIO_Port GPIOH
#define MXCHIP_SPI2_MOSI_Pin GPIO_PIN_4
#define MXCHIP_SPI2_MOSI_GPIO_Port GPIOD
#define PC14_OSC32_IN_Pin GPIO_PIN_14
#define PC14_OSC32_IN_GPIO_Port GPIOC
#define MXCHIP_SPI2_MISO_Pin GPIO_PIN_3
#define MXCHIP_SPI2_MISO_GPIO_Port GPIOD
#define PC15_OSC32_OUT_Pin GPIO_PIN_15
#define PC15_OSC32_OUT_GPIO_Port GPIOC
#define USER_Button_Pin GPIO_PIN_13
#define USER_Button_GPIO_Port GPIOC
#define LED_RED_Pin GPIO_PIN_6
#define LED_RED_GPIO_Port GPIOH
#define MXCHIP_SPI2_SCK_Pin GPIO_PIN_1
#define MXCHIP_SPI2_SCK_GPIO_Port GPIOD
#define LED_GREEN_Pin GPIO_PIN_7
#define LED_GREEN_GPIO_Port GPIOH
#define T_VCP_RX_Pin GPIO_PIN_10
#define T_VCP_RX_GPIO_Port GPIOA
#define T_VCP_TX_Pin GPIO_PIN_9
#define T_VCP_TX_GPIO_Port GPIOA
#define MXCHIP_NOTIFY_Pin GPIO_PIN_14
#define MXCHIP_NOTIFY_GPIO_Port GPIOD
#define MXCHIP_NOTIFY_EXTI_IRQn EXTI14_IRQn
#define MXCHIP_NSS_Pin GPIO_PIN_12
#define MXCHIP_NSS_GPIO_Port GPIOB
#define STSAFE_EN_Pin GPIO_PIN_11
#define STSAFE_EN_GPIO_Port GPIOF
#define MXCHIP_RESET_Pin GPIO_PIN_15
#define MXCHIP_RESET_GPIO_Port GPIOF

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
