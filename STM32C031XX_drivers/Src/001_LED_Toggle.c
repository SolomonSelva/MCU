/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include "stm32c031xx_gpio_driver.h"
#include "stm32c031xx.h"


#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

void delay(void)
{
	for(uint32_t i =0; i<500000 ; i++);
}

int main(void)
{
	GPIO_Handle_t GpioLed;

	GpioLed.pGPIOx =GPIOA;

	GpioLed.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_5;

	GpioLed.GPIOPinConfig.GPIO_PinMode = GPIO_MODE_OUT;

	GpioLed.GPIOPinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;

	GpioLed.GPIOPinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;

	GpioLed.GPIOPinConfig.GPIO_PinPuPdControl  = GPIO_PIN_PU;

	GPIO_PeriClkControl(GPIOA, ENABLE);

	GPIO_Init(&GpioLed);

	while (1)
	{
//		GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_5);
		delay();
	}

	return 0;




}