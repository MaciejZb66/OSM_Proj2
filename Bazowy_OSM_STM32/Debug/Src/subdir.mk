################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/dma.c \
../Src/freertos.c \
../Src/gpio.c \
../Src/ll_spi_ili9341.c \
../Src/main.c \
../Src/rng.c \
../Src/spi.c \
../Src/stm32f4xx_it.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32f4xx.c 

OBJS += \
./Src/dma.o \
./Src/freertos.o \
./Src/gpio.o \
./Src/ll_spi_ili9341.o \
./Src/main.o \
./Src/rng.o \
./Src/spi.o \
./Src/stm32f4xx_it.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32f4xx.o 

C_DEPS += \
./Src/dma.d \
./Src/freertos.d \
./Src/gpio.d \
./Src/ll_spi_ili9341.d \
./Src/main.d \
./Src/rng.d \
./Src/spi.d \
./Src/stm32f4xx_it.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DVECT_TAB_SRAM -DDEBUG -DSTM32F429xx -DUSE_FULL_LL_DRIVER -DHSE_VALUE=8000000 -DHSE_STARTUP_TIMEOUT=100 -DLSE_STARTUP_TIMEOUT=5000 -DLSE_VALUE=32768 -DEXTERNAL_CLOCK_VALUE=12288000 -DHSI_VALUE=16000000 -DLSI_VALUE=32000 -DVDD_VALUE=3300 -DPREFETCH_ENABLE=1 -DINSTRUCTION_CACHE_ENABLE=1 -DDATA_CACHE_ENABLE=1 -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/dma.cyclo ./Src/dma.d ./Src/dma.o ./Src/dma.su ./Src/freertos.cyclo ./Src/freertos.d ./Src/freertos.o ./Src/freertos.su ./Src/gpio.cyclo ./Src/gpio.d ./Src/gpio.o ./Src/gpio.su ./Src/ll_spi_ili9341.cyclo ./Src/ll_spi_ili9341.d ./Src/ll_spi_ili9341.o ./Src/ll_spi_ili9341.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/rng.cyclo ./Src/rng.d ./Src/rng.o ./Src/rng.su ./Src/spi.cyclo ./Src/spi.d ./Src/spi.o ./Src/spi.su ./Src/stm32f4xx_it.cyclo ./Src/stm32f4xx_it.d ./Src/stm32f4xx_it.o ./Src/stm32f4xx_it.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/system_stm32f4xx.cyclo ./Src/system_stm32f4xx.d ./Src/system_stm32f4xx.o ./Src/system_stm32f4xx.su

.PHONY: clean-Src

