################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/CK_GPIO.c \
../Core/Inc/CK_SPI.c \
../Core/Inc/CK_SYSTEM.c \
../Core/Inc/CK_TIME_HAL.c 

OBJS += \
./Core/Inc/CK_GPIO.o \
./Core/Inc/CK_SPI.o \
./Core/Inc/CK_SYSTEM.o \
./Core/Inc/CK_TIME_HAL.o 

C_DEPS += \
./Core/Inc/CK_GPIO.d \
./Core/Inc/CK_SPI.d \
./Core/Inc/CK_SYSTEM.d \
./Core/Inc/CK_TIME_HAL.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/%.o Core/Inc/%.su Core/Inc/%.cyclo: ../Core/Inc/%.c Core/Inc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Inc

clean-Core-2f-Inc:
	-$(RM) ./Core/Inc/CK_GPIO.cyclo ./Core/Inc/CK_GPIO.d ./Core/Inc/CK_GPIO.o ./Core/Inc/CK_GPIO.su ./Core/Inc/CK_SPI.cyclo ./Core/Inc/CK_SPI.d ./Core/Inc/CK_SPI.o ./Core/Inc/CK_SPI.su ./Core/Inc/CK_SYSTEM.cyclo ./Core/Inc/CK_SYSTEM.d ./Core/Inc/CK_SYSTEM.o ./Core/Inc/CK_SYSTEM.su ./Core/Inc/CK_TIME_HAL.cyclo ./Core/Inc/CK_TIME_HAL.d ./Core/Inc/CK_TIME_HAL.o ./Core/Inc/CK_TIME_HAL.su

.PHONY: clean-Core-2f-Inc

