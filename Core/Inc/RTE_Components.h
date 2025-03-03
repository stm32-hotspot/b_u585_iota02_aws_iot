/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file
  * @author  MCD Application Team
  * @version V2.0.0
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
#ifndef  __RTE_COMPONENTS_H__
#define  __RTE_COMPONENTS_H__

/* Defines ------------------------------------------------------------------*/
/* ARM.mbedTLS.3.1.1 */
#define RTE_Security_mbedTLS /* Security mbed TLS */
/* STMicroelectronics.X-CUBE-MEMS1.11.1.0 */
#define ISM330DHCX_ACCGYR_I2C
#define IIS2MDC_MAG_I2C
#define LPS22HH_PRESSTEMP_I2C
#define HTS221_HUMTEMP_I2C
/* STMicroelectronics.X-CUBE-SAFEA1.1.2.2 */
#define SAFEA1
#define STSAFEA110
/* lwIP.lwIP.2.3.0 */
<!-- the following content goes into file 'RTE_Components.h' -->
#define RTE_Network_Core /* Network Core */
#define RTE_Network_IPv4 /* Network IPv4 Stack */
#define RTE_Network_RTOS /* Network RTOS */
#define RTE_Network_FreeRTOS /* Network FreeRTOS */
#define RTE_Network_Interface_Ethernet /* Network Interface Ethernet */
#define RTE_Network_API /* Network API */

#endif /* __RTE_COMPONENTS_H__ */
