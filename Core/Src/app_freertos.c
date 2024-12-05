/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : app_freertos.c
 * Description        : FreeRTOS applicative file
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
#include "app_freertos.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "hw_defs.h"
#include "freertos_hooks.h"
#include "logging.h"

#include "sys_evt.h"
#include "FreeRTOS.h"
#include "task.h"
#include "kvstore.h"
#include <string.h>

#include "mx_netconn.h"

#include "coapTask.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
EventGroupHandle_t xSystemEvents = NULL;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 1024 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void vInitTask(void *pvArgs);
static void vHeartbeatTask(void *pvParameters);

/* USER CODE END FunctionPrototypes */

/* USER CODE BEGIN 5 */
void vApplicationMallocFailedHook(void)
{
  LogError("Malloc Fail\n");
  vDoSystemReset();
}
/* USER CODE END 5 */

/* USER CODE BEGIN 2 */
void vApplicationIdleHook( void )
{
  vPetWatchdog();
}
/* USER CODE END 2 */

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
  taskENTER_CRITICAL();

  LogSys( "Stack overflow in %s", pcTaskName );
  ( void ) xTask;

  vDoSystemReset();

  taskEXIT_CRITICAL();
}
/* USER CODE END 4 */

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{

}

__weak unsigned long getRunTimeCounterValue(void)
{
return 0;
}
/* USER CODE END 1 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  hw_init();

  /* Initialize uart for logging before cli is up and running */
  vInitLoggingEarly();

  vLoggingInit();

  LogInfo("HW Init Complete.");

  xSystemEvents = xEventGroupCreate();

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}
/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief Function implementing the defaultTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN defaultTask */
  BaseType_t xResult;
  int xMountStatus;

  (void) argument;
  xResult = xTaskCreate(Task_CLI, "cli", 2048, NULL, 10, NULL);
  configASSERT(xResult == pdTRUE);

  KVStore_init();

  (void) xEventGroupSetBits(xSystemEvents, EVT_MASK_FS_READY);

  xResult = xTaskCreate(vHeartbeatTask, "Heartbeat", 128, NULL,
  tskIDLE_PRIORITY, NULL);
  configASSERT(xResult == pdTRUE);

  xResult = xTaskCreate(&net_main, "MxNet", 1024, NULL, 23, NULL);
  configASSERT(xResult == pdTRUE);

  xResult = xTaskCreate(&vCoApTask, "CoAP", 4096, NULL, 22, NULL);
  configASSERT(xResult == pdTRUE);

#if 0
    #if DEMO_QUALIFICATION_TEST
        xResult = xTaskCreate( run_qualification_main, "QualTest", 4096, NULL, 10, NULL );
        configASSERT( xResult == pdTRUE );
    #else
        xResult = xTaskCreate( vMQTTAgentTask, "MQTTAgent", 2048, NULL, 10, NULL );
        configASSERT( xResult == pdTRUE );

        xResult = xTaskCreate( vOTAUpdateTask, "OTAUpdate", 4096, NULL, tskIDLE_PRIORITY + 1, NULL );
        configASSERT( xResult == pdTRUE );

        xResult = xTaskCreate( vEnvironmentSensorPublishTask, "EnvSense", 1024, NULL, 6, NULL );
        configASSERT( xResult == pdTRUE );

        xResult = xTaskCreate( vMotionSensorsPublish, "MotionS", 2048, NULL, 5, NULL );
        configASSERT( xResult == pdTRUE );

        xResult = xTaskCreate( vShadowDeviceTask, "ShadowDevice", 1024, NULL, 5, NULL );
        configASSERT( xResult == pdTRUE );

        xResult = xTaskCreate( vDefenderAgentTask, "AWSDefender", 2048, NULL, 5, NULL );
        configASSERT( xResult == pdTRUE );
    #endif /* DEMO_QUALIFICATION_TEST */
#endif

  /* Infinite loop */
  for (;;)
  {
    vTaskSuspend( NULL);
    osDelay(1);
  }
  /* USER CODE END defaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
static void vHeartbeatTask(void *pvParameters)
{
  (void) pvParameters;

  HAL_GPIO_WritePin( LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin( LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);

  while (1)
  {
    vTaskDelay(pdMS_TO_TICKS(1000));
    HAL_GPIO_TogglePin( LED_GREEN_GPIO_Port, LED_GREEN_Pin);
  }
}
/* USER CODE END Application */

