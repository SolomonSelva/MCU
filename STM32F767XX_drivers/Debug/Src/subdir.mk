################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/007_SPI_Rx_Master_IT.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/007_SPI_Rx_Master_IT.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/007_SPI_Rx_Master_IT.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DSTM32 -DNUCLEO_F767ZI -DSTM32F7 -DSTM32F767ZITx -c -I../Inc -I"D:/Solomon/SelfLearning/Udemy/MCU/STM32F767XX_drivers/drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/007_SPI_Rx_Master_IT.cyclo ./Src/007_SPI_Rx_Master_IT.d ./Src/007_SPI_Rx_Master_IT.o ./Src/007_SPI_Rx_Master_IT.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

