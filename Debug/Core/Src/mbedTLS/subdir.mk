################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/mbedTLS/hardware_rng.c 

OBJS += \
./Core/Src/mbedTLS/hardware_rng.o 

C_DEPS += \
./Core/Src/mbedTLS/hardware_rng.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/mbedTLS/%.o Core/Src/mbedTLS/%.su Core/Src/mbedTLS/%.cyclo: ../Core/Src/mbedTLS/%.c Core/Src/mbedTLS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -DSTM32U585xx '-DMBEDTLS_CONFIG_FILE="config_mbedtls.h"' -DNO_STSAFE -c -I../Core/Inc -I"C:/Users/stred/OneDrive/Documents/aware_demo/Middlewares/Third_Party/ARM_Security/library" -I"C:/Users/stred/OneDrive/Documents/aware_demo/Core/Src/mbedTLS" -I../Drivers/STM32U5xx_HAL_Driver/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include/ -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM33_NTZ/non_secure/ -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/ -I../Middlewares/Third_Party/CMSIS/RTOS2/Include/ -I../Middlewares/Third_Party/lwIP_Network_lwIP/rte/include/ -I../Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/include/ -I../Middlewares/Third_Party/lwIP_Network_lwIP/rte/config -I../Middlewares/Third_Party/lwIP_Network_lwIP/ports/cmsis-rtos/include/ -I../Middlewares/Third_Party/lwIP_Network_lwIP/ports/cmsis-driver/config -I../Middlewares/Third_Party/lwIP_Network_lwIP/ports/cmsis-driver/netif -I../Middlewares/Third_Party/ARM_Security/include/ -I../Middlewares/Third_Party/ARM_Security/RTE/include/ -I../Middlewares/Third_Party/ARM_Security/RTE/configs -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-mbedTLS

clean-Core-2f-Src-2f-mbedTLS:
	-$(RM) ./Core/Src/mbedTLS/hardware_rng.cyclo ./Core/Src/mbedTLS/hardware_rng.d ./Core/Src/mbedTLS/hardware_rng.o ./Core/Src/mbedTLS/hardware_rng.su

.PHONY: clean-Core-2f-Src-2f-mbedTLS

