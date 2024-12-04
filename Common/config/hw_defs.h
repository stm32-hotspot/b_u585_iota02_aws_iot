#ifndef __HW_DEFS
#define __HW_DEFS

#include "main.h"

void hw_init( void );

extern SPI_HandleTypeDef * pxHndlSpi2;
extern DMA_HandleTypeDef * pxHndlGpdmaCh4;
extern DMA_HandleTypeDef * pxHndlGpdmaCh5;

extern SPI_HandleTypeDef MXCHIP_SPI;
extern DMA_HandleTypeDef handle_GPDMA1_Channel5;
extern DMA_HandleTypeDef handle_GPDMA1_Channel4;

#define pxHndlSpi2 (&MXCHIP_SPI)
#define pxHndlGpdmaCh4 (&handle_GPDMA1_Channel4)
#define pxHndlGpdmaCh5 (&handle_GPDMA1_Channel5)


#if  configGENERATE_RUN_TIME_STATS
extern TIM_HandleTypeDef RunTimeStats_Timer;
#define pRunTimeStats_Timer (&RunTimeStats_Timer)
#endif

#if defined (HAL_ICACHE_MODULE_ENABLED)
extern DCACHE_HandleTypeDef hdcache1;
#endif

#if defined(HAL_IWDG_MODULE_ENABLED)
extern IWDG_HandleTypeDef hiwdg;
#define vPetWatchdog() HAL_IWDG_Refresh( &hiwdg )
#else
#define vPetWatchdog()
#endif

typedef void ( * GPIOInterruptCallback_t ) ( void * pvContext );

void GPIO_EXTI_Register_Callback( uint16_t usGpioPinMask, GPIOInterruptCallback_t pvCallback, void * pvContext );

void vDoSystemReset( void );


#endif /* __HW_DEFS */
