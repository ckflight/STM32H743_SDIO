################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/USBD_CDC/CK_USBD_BUFFER.c \
../Core/Inc/USBD_CDC/usb_device.c \
../Core/Inc/USBD_CDC/usbd_cdc.c \
../Core/Inc/USBD_CDC/usbd_cdc_if.c \
../Core/Inc/USBD_CDC/usbd_conf.c \
../Core/Inc/USBD_CDC/usbd_core.c \
../Core/Inc/USBD_CDC/usbd_ctlreq.c \
../Core/Inc/USBD_CDC/usbd_desc.c \
../Core/Inc/USBD_CDC/usbd_ioreq.c 

OBJS += \
./Core/Inc/USBD_CDC/CK_USBD_BUFFER.o \
./Core/Inc/USBD_CDC/usb_device.o \
./Core/Inc/USBD_CDC/usbd_cdc.o \
./Core/Inc/USBD_CDC/usbd_cdc_if.o \
./Core/Inc/USBD_CDC/usbd_conf.o \
./Core/Inc/USBD_CDC/usbd_core.o \
./Core/Inc/USBD_CDC/usbd_ctlreq.o \
./Core/Inc/USBD_CDC/usbd_desc.o \
./Core/Inc/USBD_CDC/usbd_ioreq.o 

C_DEPS += \
./Core/Inc/USBD_CDC/CK_USBD_BUFFER.d \
./Core/Inc/USBD_CDC/usb_device.d \
./Core/Inc/USBD_CDC/usbd_cdc.d \
./Core/Inc/USBD_CDC/usbd_cdc_if.d \
./Core/Inc/USBD_CDC/usbd_conf.d \
./Core/Inc/USBD_CDC/usbd_core.d \
./Core/Inc/USBD_CDC/usbd_ctlreq.d \
./Core/Inc/USBD_CDC/usbd_desc.d \
./Core/Inc/USBD_CDC/usbd_ioreq.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/USBD_CDC/%.o Core/Inc/USBD_CDC/%.su Core/Inc/USBD_CDC/%.cyclo: ../Core/Inc/USBD_CDC/%.c Core/Inc/USBD_CDC/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H743xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Inc-2f-USBD_CDC

clean-Core-2f-Inc-2f-USBD_CDC:
	-$(RM) ./Core/Inc/USBD_CDC/CK_USBD_BUFFER.cyclo ./Core/Inc/USBD_CDC/CK_USBD_BUFFER.d ./Core/Inc/USBD_CDC/CK_USBD_BUFFER.o ./Core/Inc/USBD_CDC/CK_USBD_BUFFER.su ./Core/Inc/USBD_CDC/usb_device.cyclo ./Core/Inc/USBD_CDC/usb_device.d ./Core/Inc/USBD_CDC/usb_device.o ./Core/Inc/USBD_CDC/usb_device.su ./Core/Inc/USBD_CDC/usbd_cdc.cyclo ./Core/Inc/USBD_CDC/usbd_cdc.d ./Core/Inc/USBD_CDC/usbd_cdc.o ./Core/Inc/USBD_CDC/usbd_cdc.su ./Core/Inc/USBD_CDC/usbd_cdc_if.cyclo ./Core/Inc/USBD_CDC/usbd_cdc_if.d ./Core/Inc/USBD_CDC/usbd_cdc_if.o ./Core/Inc/USBD_CDC/usbd_cdc_if.su ./Core/Inc/USBD_CDC/usbd_conf.cyclo ./Core/Inc/USBD_CDC/usbd_conf.d ./Core/Inc/USBD_CDC/usbd_conf.o ./Core/Inc/USBD_CDC/usbd_conf.su ./Core/Inc/USBD_CDC/usbd_core.cyclo ./Core/Inc/USBD_CDC/usbd_core.d ./Core/Inc/USBD_CDC/usbd_core.o ./Core/Inc/USBD_CDC/usbd_core.su ./Core/Inc/USBD_CDC/usbd_ctlreq.cyclo ./Core/Inc/USBD_CDC/usbd_ctlreq.d ./Core/Inc/USBD_CDC/usbd_ctlreq.o ./Core/Inc/USBD_CDC/usbd_ctlreq.su ./Core/Inc/USBD_CDC/usbd_desc.cyclo ./Core/Inc/USBD_CDC/usbd_desc.d ./Core/Inc/USBD_CDC/usbd_desc.o ./Core/Inc/USBD_CDC/usbd_desc.su ./Core/Inc/USBD_CDC/usbd_ioreq.cyclo ./Core/Inc/USBD_CDC/usbd_ioreq.d ./Core/Inc/USBD_CDC/usbd_ioreq.o ./Core/Inc/USBD_CDC/usbd_ioreq.su

.PHONY: clean-Core-2f-Inc-2f-USBD_CDC

