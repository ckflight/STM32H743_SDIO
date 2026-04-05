################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/COMMON/CK_FILTERS.c \
../Core/Inc/COMMON/CK_MATH.c 

OBJS += \
./Core/Inc/COMMON/CK_FILTERS.o \
./Core/Inc/COMMON/CK_MATH.o 

C_DEPS += \
./Core/Inc/COMMON/CK_FILTERS.d \
./Core/Inc/COMMON/CK_MATH.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/COMMON/%.o: ../Core/Inc/COMMON/%.c Core/Inc/COMMON/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Inc-2f-COMMON

clean-Core-2f-Inc-2f-COMMON:
	-$(RM) ./Core/Inc/COMMON/CK_FILTERS.d ./Core/Inc/COMMON/CK_FILTERS.o ./Core/Inc/COMMON/CK_MATH.d ./Core/Inc/COMMON/CK_MATH.o

.PHONY: clean-Core-2f-Inc-2f-COMMON

