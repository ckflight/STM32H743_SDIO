################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/COMMUNICATION/USBD_CDC/CK_USBD_BUFFER.c \
../Core/Inc/COMMUNICATION/USBD_CDC/usb_device.c \
../Core/Inc/COMMUNICATION/USBD_CDC/usbd_cdc.c \
../Core/Inc/COMMUNICATION/USBD_CDC/usbd_cdc_if.c \
../Core/Inc/COMMUNICATION/USBD_CDC/usbd_conf.c \
../Core/Inc/COMMUNICATION/USBD_CDC/usbd_core.c \
../Core/Inc/COMMUNICATION/USBD_CDC/usbd_ctlreq.c \
../Core/Inc/COMMUNICATION/USBD_CDC/usbd_desc.c \
../Core/Inc/COMMUNICATION/USBD_CDC/usbd_ioreq.c 

OBJS += \
./Core/Inc/COMMUNICATION/USBD_CDC/CK_USBD_BUFFER.o \
./Core/Inc/COMMUNICATION/USBD_CDC/usb_device.o \
./Core/Inc/COMMUNICATION/USBD_CDC/usbd_cdc.o \
./Core/Inc/COMMUNICATION/USBD_CDC/usbd_cdc_if.o \
./Core/Inc/COMMUNICATION/USBD_CDC/usbd_conf.o \
./Core/Inc/COMMUNICATION/USBD_CDC/usbd_core.o \
./Core/Inc/COMMUNICATION/USBD_CDC/usbd_ctlreq.o \
./Core/Inc/COMMUNICATION/USBD_CDC/usbd_desc.o \
./Core/Inc/COMMUNICATION/USBD_CDC/usbd_ioreq.o 

C_DEPS += \
./Core/Inc/COMMUNICATION/USBD_CDC/CK_USBD_BUFFER.d \
./Core/Inc/COMMUNICATION/USBD_CDC/usb_device.d \
./Core/Inc/COMMUNICATION/USBD_CDC/usbd_cdc.d \
./Core/Inc/COMMUNICATION/USBD_CDC/usbd_cdc_if.d \
./Core/Inc/COMMUNICATION/USBD_CDC/usbd_conf.d \
./Core/Inc/COMMUNICATION/USBD_CDC/usbd_core.d \
./Core/Inc/COMMUNICATION/USBD_CDC/usbd_ctlreq.d \
./Core/Inc/COMMUNICATION/USBD_CDC/usbd_desc.d \
./Core/Inc/COMMUNICATION/USBD_CDC/usbd_ioreq.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/COMMUNICATION/USBD_CDC/%.o: ../Core/Inc/COMMUNICATION/USBD_CDC/%.c Core/Inc/COMMUNICATION/USBD_CDC/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Inc-2f-COMMUNICATION-2f-USBD_CDC

clean-Core-2f-Inc-2f-COMMUNICATION-2f-USBD_CDC:
	-$(RM) ./Core/Inc/COMMUNICATION/USBD_CDC/CK_USBD_BUFFER.d ./Core/Inc/COMMUNICATION/USBD_CDC/CK_USBD_BUFFER.o ./Core/Inc/COMMUNICATION/USBD_CDC/usb_device.d ./Core/Inc/COMMUNICATION/USBD_CDC/usb_device.o ./Core/Inc/COMMUNICATION/USBD_CDC/usbd_cdc.d ./Core/Inc/COMMUNICATION/USBD_CDC/usbd_cdc.o ./Core/Inc/COMMUNICATION/USBD_CDC/usbd_cdc_if.d ./Core/Inc/COMMUNICATION/USBD_CDC/usbd_cdc_if.o ./Core/Inc/COMMUNICATION/USBD_CDC/usbd_conf.d ./Core/Inc/COMMUNICATION/USBD_CDC/usbd_conf.o ./Core/Inc/COMMUNICATION/USBD_CDC/usbd_core.d ./Core/Inc/COMMUNICATION/USBD_CDC/usbd_core.o ./Core/Inc/COMMUNICATION/USBD_CDC/usbd_ctlreq.d ./Core/Inc/COMMUNICATION/USBD_CDC/usbd_ctlreq.o ./Core/Inc/COMMUNICATION/USBD_CDC/usbd_desc.d ./Core/Inc/COMMUNICATION/USBD_CDC/usbd_desc.o ./Core/Inc/COMMUNICATION/USBD_CDC/usbd_ioreq.d ./Core/Inc/COMMUNICATION/USBD_CDC/usbd_ioreq.o

.PHONY: clean-Core-2f-Inc-2f-COMMUNICATION-2f-USBD_CDC

