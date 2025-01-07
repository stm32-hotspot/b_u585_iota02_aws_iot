#ifndef __HW_DEFS
#define __HW_DEFS

#include "main.h"

void hw_init( void );

typedef void ( * GPIOInterruptCallback_t ) ( void * pvContext );

void GPIO_EXTI_Register_Callback( uint16_t usGpioPinMask, GPIOInterruptCallback_t pvCallback, void * pvContext );

void vDoSystemReset( void );


#endif /* __HW_DEFS */
