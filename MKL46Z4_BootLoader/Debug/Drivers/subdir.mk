################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Clock.c \
../Drivers/Flash.c \
../Drivers/GPIO.c \
../Drivers/Port.c \
../Drivers/UART.c 

C_DEPS += \
./Drivers/Clock.d \
./Drivers/Flash.d \
./Drivers/GPIO.d \
./Drivers/Port.d \
./Drivers/UART.d 

OBJS += \
./Drivers/Clock.o \
./Drivers/Flash.o \
./Drivers/GPIO.o \
./Drivers/Port.o \
./Drivers/UART.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/%.o: ../Drivers/%.c Drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL46Z256VLL4_cm0plus -DCPU_MKL46Z256VLL4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\EspressoWorkspace\MKL46Z4_BootLoader\source" -I"C:\EspressoWorkspace\MKL46Z4_BootLoader\Drivers" -I"C:\EspressoWorkspace\MKL46Z4_BootLoader\Middleware" -I"C:\EspressoWorkspace\MKL46Z4_BootLoader" -I"C:\EspressoWorkspace\MKL46Z4_BootLoader\CMSIS" -I"C:\EspressoWorkspace\MKL46Z4_BootLoader\startup" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-Drivers

clean-Drivers:
	-$(RM) ./Drivers/Clock.d ./Drivers/Clock.o ./Drivers/Flash.d ./Drivers/Flash.o ./Drivers/GPIO.d ./Drivers/GPIO.o ./Drivers/Port.d ./Drivers/Port.o ./Drivers/UART.d ./Drivers/UART.o

.PHONY: clean-Drivers

