/* STM32u5 Common-IO board file */

#ifndef _COMMON_IO_BOAD_U5_
#define _COMMON_IO_BOAD_U5_

#include "main.h"

#include "iot_gpio_stm32_prv.h"

typedef enum GpioPin
{
    GPIO_MX_FLOW,
    GPIO_MX_RESET,
    GPIO_MX_NSS,
    GPIO_MX_NOTIFY,
    GPIO_MAX
} GpioPin_t;

extern const IotMappedPin_t xGpioMap[ GPIO_MAX ];

#endif /* _COMMON_IO_BOAD_U5_ */
