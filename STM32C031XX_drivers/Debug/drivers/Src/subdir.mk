################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/Src/main.c \
../drivers/Src/stm32c031xx_gpio_driver.c 

OBJS += \
./drivers/Src/main.o \
./drivers/Src/stm32c031xx_gpio_driver.o 

C_DEPS += \
./drivers/Src/main.d \
./drivers/Src/stm32c031xx_gpio_driver.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/Src/%.o drivers/Src/%.su drivers/Src/%.cyclo: ../drivers/Src/%.c drivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DNUCLEO_C031C6 -DSTM32 -DSTM32C0 -DSTM32C031C6Tx -c -I../Inc -I"D:/Solomon/SelfLearning/Udemy/MCU/STM32C031XX_drivers/drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-drivers-2f-Src

clean-drivers-2f-Src:
	-$(RM) ./drivers/Src/main.cyclo ./drivers/Src/main.d ./drivers/Src/main.o ./drivers/Src/main.su ./drivers/Src/stm32c031xx_gpio_driver.cyclo ./drivers/Src/stm32c031xx_gpio_driver.d ./drivers/Src/stm32c031xx_gpio_driver.o ./drivers/Src/stm32c031xx_gpio_driver.su

.PHONY: clean-drivers-2f-Src

