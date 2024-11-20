################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/lwIP_Network_lwIP/ports/cmsis-driver/netif/ethernetif.c 

OBJS += \
./Middlewares/Third_Party/lwIP_Network_lwIP/ports/cmsis-driver/netif/ethernetif.o 

C_DEPS += \
./Middlewares/Third_Party/lwIP_Network_lwIP/ports/cmsis-driver/netif/ethernetif.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/lwIP_Network_lwIP/ports/cmsis-driver/netif/%.o Middlewares/Third_Party/lwIP_Network_lwIP/ports/cmsis-driver/netif/%.su Middlewares/Third_Party/lwIP_Network_lwIP/ports/cmsis-driver/netif/%.cyclo: ../Middlewares/Third_Party/lwIP_Network_lwIP/ports/cmsis-driver/netif/%.c Middlewares/Third_Party/lwIP_Network_lwIP/ports/cmsis-driver/netif/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -DSTM32U585xx -c -I../Core/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include/ -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM33_NTZ/non_secure/ -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/ -I../Middlewares/Third_Party/CMSIS/RTOS2/Include/ -I../Middlewares/Third_Party/lwIP_Network_lwIP/rte/include/ -I../Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/include/ -I../Middlewares/Third_Party/lwIP_Network_lwIP/rte/config -I../Middlewares/Third_Party/lwIP_Network_lwIP/ports/cmsis-rtos/include/ -I../Middlewares/Third_Party/lwIP_Network_lwIP/ports/cmsis-driver/config -I../Middlewares/Third_Party/lwIP_Network_lwIP/ports/cmsis-driver/netif -I../Middlewares/Third_Party/ARM_Security/include/ -I../Middlewares/Third_Party/ARM_Security/RTE/include/ -I../Middlewares/Third_Party/ARM_Security/RTE/configs -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-lwIP_Network_lwIP-2f-ports-2f-cmsis-2d-driver-2f-netif

clean-Middlewares-2f-Third_Party-2f-lwIP_Network_lwIP-2f-ports-2f-cmsis-2d-driver-2f-netif:
	-$(RM) ./Middlewares/Third_Party/lwIP_Network_lwIP/ports/cmsis-driver/netif/ethernetif.cyclo ./Middlewares/Third_Party/lwIP_Network_lwIP/ports/cmsis-driver/netif/ethernetif.d ./Middlewares/Third_Party/lwIP_Network_lwIP/ports/cmsis-driver/netif/ethernetif.o ./Middlewares/Third_Party/lwIP_Network_lwIP/ports/cmsis-driver/netif/ethernetif.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-lwIP_Network_lwIP-2f-ports-2f-cmsis-2d-driver-2f-netif

