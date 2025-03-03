################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Src/Startup/startup_stm32f767zitx.s 

OBJS += \
./Src/Startup/startup_stm32f767zitx.o 

S_DEPS += \
./Src/Startup/startup_stm32f767zitx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/Startup/%.o: ../Src/Startup/%.s Src/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

clean: clean-Src-2f-Startup

clean-Src-2f-Startup:
	-$(RM) ./Src/Startup/startup_stm32f767zitx.d ./Src/Startup/startup_stm32f767zitx.o

.PHONY: clean-Src-2f-Startup

