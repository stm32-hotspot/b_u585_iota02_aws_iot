/*
 * Copyright of Amazon Web Services, Inc. (AWS) 2022
 *
 * This code is licensed under the AWS Intellectual Property License, which can
 * be found here: https://aws.amazon.com/legal/aws-ip-license-terms/; provided
 * that AWS grants you a limited, royalty-free, revocable, non-exclusive,
 * non-sublicensable, non-transferrable license to modify the code and
 * distribute binaries produced from the code (or modified versions of the code)
 * within hardware modules provided to third parties as long as such hardware
 * modules are qualified for AWS IoT ExpressLink under the AWS Device
 * Qualification Program (https://aws.amazon.com/partners/programs/dqp/). Your
 * receipt of this code is subject to any non-disclosure (or similar) agreement
 * between you and AWS.
 */
#include "main.h"

#include "logging_levels.h"
#define LOG_LEVEL    LOG_ERROR
#include "logging.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "freertos_hooks.h"
#include <string.h>

#if  configGENERATE_RUN_TIME_STATS
extern TIM_HandleTypeDef htim2;
#endif

#if defined(USE_IWDG)
extern IWDG_HandleTypeDef hiwdg;
#endif

void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     StackType_t ** ppxTimerTaskStackBuffer,
                                     uint32_t * pulTimerTaskStackSize )

{
    static StaticTask_t timerTaskTCB;
    static StackType_t timerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

    *ppxTimerTaskTCBBuffer = &timerTaskTCB;
    *ppxTimerTaskStackBuffer = timerTaskStack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    StackType_t ** ppxIdleTaskStackBuffer,
                                    uint32_t * pulIdleTaskStackSize )

{
    static StaticTask_t idleTaskTCB;
    static StackType_t idleTaskStack[ configMINIMAL_STACK_SIZE ];

    *ppxIdleTaskTCBBuffer = &idleTaskTCB;
    *ppxIdleTaskStackBuffer = idleTaskStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

#if configUSE_IDLE_HOOK == 1
void vApplicationIdleHook( void )
{
  vPetWatchdog();
  __WFI();
}
#endif

#if configUSE_MALLOC_FAILED_HOOK
void vApplicationMallocFailedHook(void)
{
	LogError("Malloc Fail\n");
	vDoSystemReset();
}
#endif

#if (configCHECK_FOR_STACK_OVERFLOW > 0)
void vApplicationStackOverflowHook( TaskHandle_t xTask, char * pcTaskName )
{
    volatile uint32_t ulSetToZeroToStepOut = 1UL;

    taskENTER_CRITICAL();

    LogSys( "Stack overflow in %s", pcTaskName );
    ( void ) xTask;

    vDoSystemReset();

    taskEXIT_CRITICAL();
}
#endif

#if (configUSE_DAEMON_TASK_STARTUP_HOOK == 1)
void vApplicationDaemonTaskStartupHook (void)
{

}
#endif

void vDoSystemReset( void )
{
    vPetWatchdog();

    if( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
    {
        vTaskSuspendAll();
    }

    LogSys( "System Reset in progress." );

    /* Drain log buffers */
    vDyingGasp();

    NVIC_SystemReset();
}

#if  configGENERATE_RUN_TIME_STATS
void configureTimerForRunTimeStats(void)
{
  HAL_TIM_Base_Stop(&htim2);
  htim2.Instance->CNT = 0;
  HAL_TIM_Base_Start(&htim2);
}

configRUN_TIME_COUNTER_TYPE getRunTimeCounterValue(void)
{
  static configRUN_TIME_COUNTER_TYPE counter = 0;
  static uint32_t last_cnt_val = 0;
   uint32_t current_cnt_val = 0;
  uint32_t difference;

  current_cnt_val = htim2.Instance->CNT;

  difference = current_cnt_val - last_cnt_val;

  last_cnt_val= current_cnt_val;

  counter += difference;

  return counter;
}

static configRUN_TIME_COUNTER_TYPE IRQ_Timer = 0;

void incrementIRQRunTime(configRUN_TIME_COUNTER_TYPE Initial_time)
{
  IRQ_Timer  += getRunTimeCounterValue() - Initial_time;
}

configRUN_TIME_COUNTER_TYPE getIRQTimeCounterValue(void)
{
  return IRQ_Timer;
}
#endif


typedef void ( * VectorTable_t )( void );

#define NUM_USER_IRQ               ( FMAC_IRQn + 1 )      /* MCU specific */
#define VECTOR_TABLE_SIZE          ( NVIC_USER_IRQ_OFFSET + NUM_USER_IRQ )
#define VECTOR_TABLE_ALIGN_CM33    0x400U

static VectorTable_t pulVectorTableSRAM[ VECTOR_TABLE_SIZE ] __attribute__( ( aligned( VECTOR_TABLE_ALIGN_CM33 ) ) );

extern DCACHE_HandleTypeDef hdcache1;
#define pxHndlDCache &hdcache1
/* Relocate vector table to ram for runtime interrupt registration */
void vRelocateVectorTable( void )
{
    /* Disable interrupts */
    __disable_irq();

//    HAL_ICACHE_Disable();
//    HAL_DCACHE_Disable( pxHndlDCache );

    /* Copy vector table to ram */
    ( void ) memcpy( pulVectorTableSRAM, ( uint32_t * ) SCB->VTOR, sizeof( uint32_t ) * VECTOR_TABLE_SIZE );

    SCB->VTOR = ( uint32_t ) pulVectorTableSRAM;

    __DSB();
    __ISB();

//    HAL_DCACHE_Invalidate( pxHndlDCache );
//    HAL_ICACHE_Invalidate();
//    HAL_ICACHE_Enable();
//    HAL_DCACHE_Enable( pxHndlDCache );

    __enable_irq();
}
