/* STM32u5 Common-IO board file */
#include "iot_gpio_stm32_prv.h"
#include "mx_gpio.h"

#include "main.h"

const IotMappedPin_t xGpioMap[ GPIO_MAX ] =
{
    { MXCHIP_FLOW_GPIO_Port  , MXCHIP_FLOW_Pin  , MXCHIP_FLOW_EXTI_IRQn   }, /* GPIO_MX_FLOW     */
    { MXCHIP_RESET_GPIO_Port , MXCHIP_RESET_Pin , 0                       }, /* GPIO_MX_RESET    */
    { MXCHIP_NSS_GPIO_Port   , MXCHIP_NSS_Pin   , 0                       }, /* GPIO_MX_NSS      */
    { MXCHIP_NOTIFY_GPIO_Port, MXCHIP_NOTIFY_Pin, MXCHIP_NOTIFY_EXTI_IRQn }  /* GPIO_MX_NOTIFY   */
};
