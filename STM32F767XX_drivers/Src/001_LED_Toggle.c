/*
 * 001_LED_Toggle.c
 *
 *  Created on: Nov 15, 2024
 *      Author: solom
 */


#include <stdint.h>
#include <stdio.h>
#include "stm32f767xx_gpio_driver.h"
#include "stm32f767xx.h"


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

	GpioLed.pGPIOx =GPIOB;

	GpioLed.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_7;

	GpioLed.GPIOPinConfig.GPIO_PinMode = GPIO_MODE_OUT;

	GpioLed.GPIOPinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;

	GpioLed.GPIOPinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;

	GpioLed.GPIOPinConfig.GPIO_PinPuPdControl  = GPIO_NO_PP;

	GPIO_PeriClkControl(GPIOB, ENABLE);

	GPIO_Init(&GpioLed);

	while (1)
	{
//		GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		GPIO_ToggleOutputPin(GPIOB, GPIO_PIN_7);
		delay();
		//printf("Hello WOrld \n");
	}

	return 0;




}

