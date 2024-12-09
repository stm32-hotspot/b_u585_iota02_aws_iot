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

#include "lfs.h"
#include "lfs_port.h"

#include "mx_netconn.h"

#include "lwip/sockets.h"

#include "echo.h"
#include "mqtt_agent_task.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ECHO_PORT 7
//#define REMOTE_IP_ADDRESS "192.168.1.25"
#define REMOTE_IP_ADDRESS "192.168.137.173"
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
EventGroupHandle_t xSystemEvents = NULL;
static lfs_t *pxLfsCtx = NULL;

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
static int fs_init(void);
lfs_t* pxGetDefaultFsCtx(void);
void vReceiverTask(void *pvParameters);
void lwip_socket_send(const char *message, const char *dest_ip, uint16_t dest_port);
void vMainTask(void *pvParameters);

extern void vSubscribePublishTestTask(void*);
extern void vDefenderAgentTask( void * pvParameters );
extern void vShadowDeviceTask( void * pvParameters );
extern void vOTAUpdateTask( void * pvParam );
/* USER CODE END FunctionPrototypes */

/* USER CODE BEGIN 5 */
void vApplicationMallocFailedHook(void)
{
  LogError("Malloc Fail\n");
  vDoSystemReset();
}
/* USER CODE END 5 */

/* USER CODE BEGIN 2 */
void vApplicationIdleHook(void)
{
  vPetWatchdog();
}
/* USER CODE END 2 */

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
  taskENTER_CRITICAL();

  LogSys("Stack overflow in %s", pcTaskName);
  (void) xTask;

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

#if KV_STORE_NVIMPL_LITTLEFS
  xMountStatus = fs_init();

  if (xMountStatus == LFS_ERR_OK)
  {
    /*
     * FIXME: Need to debug  the cause of internal flash status register error here.
     * Clearing the flash status register as a workaround.
     */
    FLASH_WaitForLastOperation(1000);

    LogInfo( "File System mounted." );
#if 0
      otaPal_EarlyInit();
#endif
    (void) xEventGroupSetBits(xSystemEvents, EVT_MASK_FS_READY);

    KVStore_init();
  }
  else
  {
    LogError("Failed to mount filesystem.");
  }
#else
  KVStore_init();
#endif

  (void) xEventGroupSetBits(xSystemEvents, EVT_MASK_FS_READY);

  xResult = xTaskCreate(vHeartbeatTask, "Heartbeat", 128, NULL,
  tskIDLE_PRIORITY, NULL);
  configASSERT(xResult == pdTRUE);

  xResult = xTaskCreate(&net_main, "MxNet", 1024, NULL, 23, NULL);
  configASSERT(xResult == pdTRUE);

#if 0
  xResult = xTaskCreate(vEchoServerTask, "EchoServer", 1024, NULL, 22, NULL);
  configASSERT(xResult == pdTRUE);
#endif

#if DEMO_QUALIFICATION_TEST
        xResult = xTaskCreate( run_qualification_main, "QualTest", 4096, NULL, 10, NULL );
        configASSERT( xResult == pdTRUE );
    #else
  xResult = xTaskCreate(vMQTTAgentTask, "MQTTAgent", 2048, NULL, 10, NULL);
  configASSERT(xResult == pdTRUE);

  xResult = xTaskCreate(vSubscribePublishTestTask, "PubSub", 6144, NULL, 10, NULL);
  configASSERT(xResult == pdTRUE);

        xResult = xTaskCreate( vOTAUpdateTask, "OTAUpdate", 4096, NULL, tskIDLE_PRIORITY + 1, NULL );
        configASSERT( xResult == pdTRUE );
#if 0
        xResult = xTaskCreate( vEnvironmentSensorPublishTask, "EnvSense", 1024, NULL, 6, NULL );
        configASSERT( xResult == pdTRUE );

        xResult = xTaskCreate( vMotionSensorsPublish, "MotionS", 2048, NULL, 5, NULL );
        configASSERT( xResult == pdTRUE );
#endif
        xResult = xTaskCreate( vShadowDeviceTask, "ShadowDevice", 1024, NULL, 5, NULL );
        configASSERT( xResult == pdTRUE );

        xResult = xTaskCreate( vDefenderAgentTask, "AWSDefender", 2048, NULL, 5, NULL );
        configASSERT( xResult == pdTRUE );

#endif /* DEMO_QUALIFICATION_TEST */

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

lfs_t* pxGetDefaultFsCtx(void)
{
  while (pxLfsCtx == NULL)
  {
    LogDebug( "Waiting for FS Initialization." );
    /* Wait for FS to be initialized */
    vTaskDelay(1000);
    /*TODO block on an event group bit instead */
  }

  return pxLfsCtx;
}

static int fs_init(void)
{
  static lfs_t xLfsCtx =
  { 0 };

  struct lfs_info xDirInfo =
  { 0 };

  /* Block time of up to 1 s for filesystem to initialize */
//  const struct lfs_config *pxCfg = pxInitializeOSPIFlashFs(pdMS_TO_TICKS(30 * 1000));

  const struct lfs_config *pxCfg = pxInitializeInternalFlashFs(pdMS_TO_TICKS(30 * 1000));

  /* mount the filesystem */
  int err = lfs_mount(&xLfsCtx, pxCfg);

  /* format if we can't mount the filesystem
   * this should only happen on the first boot
   */
  if (err != LFS_ERR_OK)
  {
    LogError("Failed to mount partition. Formatting...");
    err = lfs_format(&xLfsCtx, pxCfg);

    if (err == 0)
    {
      err = lfs_mount(&xLfsCtx, pxCfg);
    }

    if (err != LFS_ERR_OK)
    {
      LogError("Failed to format littlefs device.");
    }
  }

  if (lfs_stat(&xLfsCtx, "/cfg", &xDirInfo) == LFS_ERR_NOENT)
  {
    err = lfs_mkdir(&xLfsCtx, "/cfg");

    if (err != LFS_ERR_OK)
    {
      LogError("Failed to create /cfg directory.");
    }
  }

  if (lfs_stat(&xLfsCtx, "/ota", &xDirInfo) == LFS_ERR_NOENT)
  {
    err = lfs_mkdir(&xLfsCtx, "/ota");

    if (err != LFS_ERR_OK)
    {
      LogError("Failed to create /ota directory.");
    }
  }

  if (err == 0)
  {
    /* Export the FS context */
    pxLfsCtx = &xLfsCtx;
  }

  return err;
}

/* USER CODE END Application */

