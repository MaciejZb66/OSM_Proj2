################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/dma.c \
../Src/freertos.c \
../Src/gpio.c \
../Src/ll_spi_ili9341.c \
../Src/main.c \
../Src/myfunctions.c \
../Src/reg2.c \
../Src/rng.c \
../Src/spi.c \
../Src/stm32f4xx_hal_msp.c \
../Src/stm32f4xx_hal_timebase_tim.c \
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
./Src/myfunctions.o \
./Src/reg2.o \
./Src/rng.o \
./Src/spi.o \
./Src/stm32f4xx_hal_msp.o \
./Src/stm32f4xx_hal_timebase_tim.o \
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
./Src/myfunctions.d \
./Src/reg2.d \
./Src/rng.d \
./Src/spi.d \
./Src/stm32f4xx_hal_msp.d \
./Src/stm32f4xx_hal_timebase_tim.d \
./Src/stm32f4xx_it.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DVECT_TAB_SRAM -DDEBUG -DSTM32F429xx -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/dma.d ./Src/dma.o ./Src/dma.su ./Src/freertos.d ./Src/freertos.o ./Src/freertos.su ./Src/gpio.d ./Src/gpio.o ./Src/gpio.su ./Src/ll_spi_ili9341.d ./Src/ll_spi_ili9341.o ./Src/ll_spi_ili9341.su ./Src/main.d ./Src/main.o ./Src/main.su ./Src/myfunctions.d ./Src/myfunctions.o ./Src/myfunctions.su ./Src/reg2.d ./Src/reg2.o ./Src/reg2.su ./Src/rng.d ./Src/rng.o ./Src/rng.su ./Src/spi.d ./Src/spi.o ./Src/spi.su ./Src/stm32f4xx_hal_msp.d ./Src/stm32f4xx_hal_msp.o ./Src/stm32f4xx_hal_msp.su ./Src/stm32f4xx_hal_timebase_tim.d ./Src/stm32f4xx_hal_timebase_tim.o ./Src/stm32f4xx_hal_timebase_tim.su ./Src/stm32f4xx_it.d ./Src/stm32f4xx_it.o ./Src/stm32f4xx_it.su ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/system_stm32f4xx.d ./Src/system_stm32f4xx.o ./Src/system_stm32f4xx.su

.PHONY: clean-Src

