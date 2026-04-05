################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/SENSORS/CK_BMP280.c \
../Core/Inc/SENSORS/CK_FXOS8700CQ.c \
../Core/Inc/SENSORS/CK_HMC5983.c \
../Core/Inc/SENSORS/CK_ICM20602.c \
../Core/Inc/SENSORS/CK_ICM42605.c \
../Core/Inc/SENSORS/CK_ICM42688P.c \
../Core/Inc/SENSORS/CK_L3GD20H.c \
../Core/Inc/SENSORS/CK_LSM303D.c \
../Core/Inc/SENSORS/CK_MAG3110.c \
../Core/Inc/SENSORS/CK_MAX7456.c \
../Core/Inc/SENSORS/CK_MLX90393.c \
../Core/Inc/SENSORS/CK_MS5607.c \
../Core/Inc/SENSORS/CK_MS5611.c \
../Core/Inc/SENSORS/CK_QMC5883L.c 

OBJS += \
./Core/Inc/SENSORS/CK_BMP280.o \
./Core/Inc/SENSORS/CK_FXOS8700CQ.o \
./Core/Inc/SENSORS/CK_HMC5983.o \
./Core/Inc/SENSORS/CK_ICM20602.o \
./Core/Inc/SENSORS/CK_ICM42605.o \
./Core/Inc/SENSORS/CK_ICM42688P.o \
./Core/Inc/SENSORS/CK_L3GD20H.o \
./Core/Inc/SENSORS/CK_LSM303D.o \
./Core/Inc/SENSORS/CK_MAG3110.o \
./Core/Inc/SENSORS/CK_MAX7456.o \
./Core/Inc/SENSORS/CK_MLX90393.o \
./Core/Inc/SENSORS/CK_MS5607.o \
./Core/Inc/SENSORS/CK_MS5611.o \
./Core/Inc/SENSORS/CK_QMC5883L.o 

C_DEPS += \
./Core/Inc/SENSORS/CK_BMP280.d \
./Core/Inc/SENSORS/CK_FXOS8700CQ.d \
./Core/Inc/SENSORS/CK_HMC5983.d \
./Core/Inc/SENSORS/CK_ICM20602.d \
./Core/Inc/SENSORS/CK_ICM42605.d \
./Core/Inc/SENSORS/CK_ICM42688P.d \
./Core/Inc/SENSORS/CK_L3GD20H.d \
./Core/Inc/SENSORS/CK_LSM303D.d \
./Core/Inc/SENSORS/CK_MAG3110.d \
./Core/Inc/SENSORS/CK_MAX7456.d \
./Core/Inc/SENSORS/CK_MLX90393.d \
./Core/Inc/SENSORS/CK_MS5607.d \
./Core/Inc/SENSORS/CK_MS5611.d \
./Core/Inc/SENSORS/CK_QMC5883L.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/SENSORS/%.o: ../Core/Inc/SENSORS/%.c Core/Inc/SENSORS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Inc-2f-SENSORS

clean-Core-2f-Inc-2f-SENSORS:
	-$(RM) ./Core/Inc/SENSORS/CK_BMP280.d ./Core/Inc/SENSORS/CK_BMP280.o ./Core/Inc/SENSORS/CK_FXOS8700CQ.d ./Core/Inc/SENSORS/CK_FXOS8700CQ.o ./Core/Inc/SENSORS/CK_HMC5983.d ./Core/Inc/SENSORS/CK_HMC5983.o ./Core/Inc/SENSORS/CK_ICM20602.d ./Core/Inc/SENSORS/CK_ICM20602.o ./Core/Inc/SENSORS/CK_ICM42605.d ./Core/Inc/SENSORS/CK_ICM42605.o ./Core/Inc/SENSORS/CK_ICM42688P.d ./Core/Inc/SENSORS/CK_ICM42688P.o ./Core/Inc/SENSORS/CK_L3GD20H.d ./Core/Inc/SENSORS/CK_L3GD20H.o ./Core/Inc/SENSORS/CK_LSM303D.d ./Core/Inc/SENSORS/CK_LSM303D.o ./Core/Inc/SENSORS/CK_MAG3110.d ./Core/Inc/SENSORS/CK_MAG3110.o ./Core/Inc/SENSORS/CK_MAX7456.d ./Core/Inc/SENSORS/CK_MAX7456.o ./Core/Inc/SENSORS/CK_MLX90393.d ./Core/Inc/SENSORS/CK_MLX90393.o ./Core/Inc/SENSORS/CK_MS5607.d ./Core/Inc/SENSORS/CK_MS5607.o ./Core/Inc/SENSORS/CK_MS5611.d ./Core/Inc/SENSORS/CK_MS5611.o ./Core/Inc/SENSORS/CK_QMC5883L.d ./Core/Inc/SENSORS/CK_QMC5883L.o

.PHONY: clean-Core-2f-Inc-2f-SENSORS

