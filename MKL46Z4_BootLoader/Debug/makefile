################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include source/subdir.mk
-include Middleware/subdir.mk
-include Drivers/subdir.mk
-include CMSIS/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := MKL46Z4_BootLoader
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: MKL46Z4_BootLoader.axf

# Tool invocations
MKL46Z4_BootLoader.axf: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -Map="MKL46Z4_BootLoader.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m0plus -mthumb -T MKL46Z4_BootLoader_Debug.ld -o "MKL46Z4_BootLoader.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) MKL46Z4_BootLoader.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "MKL46Z4_BootLoader.axf"; arm-none-eabi-objcopy -v -O binary "MKL46Z4_BootLoader.axf" "MKL46Z4_BootLoader.bin" ; arm-none-eabi-objcopy -O ihex "MKL46Z4_BootLoader.axf" "MKL46Z4_BootLoader.hex"; arm-none-eabi-objcopy -O srec "MKL46Z4_BootLoader.axf" "MKL46Z4_BootLoader.s19";# checksum -p MKL46Z256xxx4 -d "MKL46Z4_BootLoader.bin";
	-@echo ' '

.PHONY: all clean dependents main-build post-build

-include ../makefile.targets
