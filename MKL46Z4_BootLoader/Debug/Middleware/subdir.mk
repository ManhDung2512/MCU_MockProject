################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middleware/Middleware.c 

C_DEPS += \
./Middleware/Middleware.d 

OBJS += \
./Middleware/Middleware.o 


# Each subdirectory must supply rules for building sources it contributes
Middleware/%.o: ../Middleware/%.c Middleware/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL46Z256VLL4_cm0plus -DCPU_MKL46Z256VLL4 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\EspressoWorkspace\MKL46Z4_BootLoader\source" -I"C:\EspressoWorkspace\MKL46Z4_BootLoader\Drivers" -I"C:\EspressoWorkspace\MKL46Z4_BootLoader\Middleware" -I"C:\EspressoWorkspace\MKL46Z4_BootLoader" -I"C:\EspressoWorkspace\MKL46Z4_BootLoader\CMSIS" -I"C:\EspressoWorkspace\MKL46Z4_BootLoader\startup" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-Middleware

clean-Middleware:
	-$(RM) ./Middleware/Middleware.d ./Middleware/Middleware.o

.PHONY: clean-Middleware

