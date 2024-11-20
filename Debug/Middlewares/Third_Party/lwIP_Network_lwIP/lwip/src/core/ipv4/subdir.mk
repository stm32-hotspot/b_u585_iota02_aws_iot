################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/autoip.c \
../Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/dhcp.c \
../Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/etharp.c \
../Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/icmp.c \
../Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/igmp.c \
../Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/ip4.c \
../Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/ip4_addr.c \
../Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/ip4_frag.c 

OBJS += \
./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/autoip.o \
./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/dhcp.o \
./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/etharp.o \
./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/icmp.o \
./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/igmp.o \
./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/ip4.o \
./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/ip4_addr.o \
./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/ip4_frag.o 

C_DEPS += \
./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/autoip.d \
./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/dhcp.d \
./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/etharp.d \
./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/icmp.d \
./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/igmp.d \
./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/ip4.d \
./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/ip4_addr.d \
./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/ip4_frag.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/%.o Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/%.su Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/%.cyclo: ../Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/%.c Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -DSTM32U585xx '-DMBEDTLS_CONFIG_FILE="config_mbedtls.h"' -DNO_STSAFE -c -I../Core/Inc -I"C:/Users/stred/OneDrive/Documents/aware_demo/Middlewares/Third_Party/ARM_Security/library" -I"C:/Users/stred/OneDrive/Documents/aware_demo/Core/Src/mbedTLS" -I../Drivers/STM32U5xx_HAL_Driver/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include/ -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM33_NTZ/non_secure/ -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/ -I../Middlewares/Third_Party/CMSIS/RTOS2/Include/ -I../Middlewares/Third_Party/lwIP_Network_lwIP/rte/include/ -I../Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/include/ -I../Middlewares/Third_Party/lwIP_Network_lwIP/rte/config -I../Middlewares/Third_Party/lwIP_Network_lwIP/ports/cmsis-rtos/include/ -I../Middlewares/Third_Party/lwIP_Network_lwIP/ports/cmsis-driver/config -I../Middlewares/Third_Party/lwIP_Network_lwIP/ports/cmsis-driver/netif -I../Middlewares/Third_Party/ARM_Security/include/ -I../Middlewares/Third_Party/ARM_Security/RTE/include/ -I../Middlewares/Third_Party/ARM_Security/RTE/configs -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Third_Party-2f-lwIP_Network_lwIP-2f-lwip-2f-src-2f-core-2f-ipv4

clean-Middlewares-2f-Third_Party-2f-lwIP_Network_lwIP-2f-lwip-2f-src-2f-core-2f-ipv4:
	-$(RM) ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/autoip.cyclo ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/autoip.d ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/autoip.o ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/autoip.su ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/dhcp.cyclo ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/dhcp.d ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/dhcp.o ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/dhcp.su ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/etharp.cyclo ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/etharp.d ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/etharp.o ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/etharp.su ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/icmp.cyclo ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/icmp.d ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/icmp.o ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/icmp.su ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/igmp.cyclo ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/igmp.d ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/igmp.o ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/igmp.su ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/ip4.cyclo ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/ip4.d ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/ip4.o ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/ip4.su ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/ip4_addr.cyclo ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/ip4_addr.d ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/ip4_addr.o ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/ip4_addr.su ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/ip4_frag.cyclo ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/ip4_frag.d ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/ip4_frag.o ./Middlewares/Third_Party/lwIP_Network_lwIP/lwip/src/core/ipv4/ip4_frag.su

.PHONY: clean-Middlewares-2f-Third_Party-2f-lwIP_Network_lwIP-2f-lwip-2f-src-2f-core-2f-ipv4

