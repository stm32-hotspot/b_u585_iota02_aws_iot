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
/* lwIP.lwIP.2.2.0 */
<!-- the following content goes into file 'RTE_Components.h' -->
#define RTE_Network_Core /* Network Core */
#define RTE_Network_IPv4 /* Network IPv4 Stack */
#define RTE_Network_RTOS /* Network RTOS */
#define RTE_Network_CMSIS_RTOS2 /* Network CMSIS-RTOS2 */
#define RTE_Network_Interface_Ethernet /* Network Interface Ethernet */
#define RTE_Network_Driver_Ethernet /* Network Driver Ethernet */
#define RTE_Network_API /* Network API */

#endif /* __RTE_COMPONENTS_H__ */
