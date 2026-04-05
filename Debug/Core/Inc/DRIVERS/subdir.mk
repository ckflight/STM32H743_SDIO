################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/DRIVERS/CK_ADC.c \
../Core/Inc/DRIVERS/CK_BUZZER.c \
../Core/Inc/DRIVERS/CK_CIRCULARBUFFER.c \
../Core/Inc/DRIVERS/CK_GPIO.c \
../Core/Inc/DRIVERS/CK_I2C.c \
../Core/Inc/DRIVERS/CK_LED.c \
../Core/Inc/DRIVERS/CK_MICROCARD.c \
../Core/Inc/DRIVERS/CK_RGB.c \
../Core/Inc/DRIVERS/CK_SPI.c \
../Core/Inc/DRIVERS/CK_SPI_DMA.c \
../Core/Inc/DRIVERS/CK_SYSTEM.c \
../Core/Inc/DRIVERS/CK_TIME_HAL.c \
../Core/Inc/DRIVERS/CK_UART.c 

OBJS += \
./Core/Inc/DRIVERS/CK_ADC.o \
./Core/Inc/DRIVERS/CK_BUZZER.o \
./Core/Inc/DRIVERS/CK_CIRCULARBUFFER.o \
./Core/Inc/DRIVERS/CK_GPIO.o \
./Core/Inc/DRIVERS/CK_I2C.o \
./Core/Inc/DRIVERS/CK_LED.o \
./Core/Inc/DRIVERS/CK_MICROCARD.o \
./Core/Inc/DRIVERS/CK_RGB.o \
./Core/Inc/DRIVERS/CK_SPI.o \
./Core/Inc/DRIVERS/CK_SPI_DMA.o \
./Core/Inc/DRIVERS/CK_SYSTEM.o \
./Core/Inc/DRIVERS/CK_TIME_HAL.o \
./Core/Inc/DRIVERS/CK_UART.o 

C_DEPS += \
./Core/Inc/DRIVERS/CK_ADC.d \
./Core/Inc/DRIVERS/CK_BUZZER.d \
./Core/Inc/DRIVERS/CK_CIRCULARBUFFER.d \
./Core/Inc/DRIVERS/CK_GPIO.d \
./Core/Inc/DRIVERS/CK_I2C.d \
./Core/Inc/DRIVERS/CK_LED.d \
./Core/Inc/DRIVERS/CK_MICROCARD.d \
./Core/Inc/DRIVERS/CK_RGB.d \
./Core/Inc/DRIVERS/CK_SPI.d \
./Core/Inc/DRIVERS/CK_SPI_DMA.d \
./Core/Inc/DRIVERS/CK_SYSTEM.d \
./Core/Inc/DRIVERS/CK_TIME_HAL.d \
./Core/Inc/DRIVERS/CK_UART.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/DRIVERS/%.o: ../Core/Inc/DRIVERS/%.c Core/Inc/DRIVERS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Inc-2f-DRIVERS

clean-Core-2f-Inc-2f-DRIVERS:
	-$(RM) ./Core/Inc/DRIVERS/CK_ADC.d ./Core/Inc/DRIVERS/CK_ADC.o ./Core/Inc/DRIVERS/CK_BUZZER.d ./Core/Inc/DRIVERS/CK_BUZZER.o ./Core/Inc/DRIVERS/CK_CIRCULARBUFFER.d ./Core/Inc/DRIVERS/CK_CIRCULARBUFFER.o ./Core/Inc/DRIVERS/CK_GPIO.d ./Core/Inc/DRIVERS/CK_GPIO.o ./Core/Inc/DRIVERS/CK_I2C.d ./Core/Inc/DRIVERS/CK_I2C.o ./Core/Inc/DRIVERS/CK_LED.d ./Core/Inc/DRIVERS/CK_LED.o ./Core/Inc/DRIVERS/CK_MICROCARD.d ./Core/Inc/DRIVERS/CK_MICROCARD.o ./Core/Inc/DRIVERS/CK_RGB.d ./Core/Inc/DRIVERS/CK_RGB.o ./Core/Inc/DRIVERS/CK_SPI.d ./Core/Inc/DRIVERS/CK_SPI.o ./Core/Inc/DRIVERS/CK_SPI_DMA.d ./Core/Inc/DRIVERS/CK_SPI_DMA.o ./Core/Inc/DRIVERS/CK_SYSTEM.d ./Core/Inc/DRIVERS/CK_SYSTEM.o ./Core/Inc/DRIVERS/CK_TIME_HAL.d ./Core/Inc/DRIVERS/CK_TIME_HAL.o ./Core/Inc/DRIVERS/CK_UART.d ./Core/Inc/DRIVERS/CK_UART.o

.PHONY: clean-Core-2f-Inc-2f-DRIVERS

