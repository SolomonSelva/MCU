/*
 * stm32f767xx_dwt_driver.c
 *
 *  Created on: Nov 16, 2024
 *      Author: root
 */

#include "stm32f767xx.h"
#include "stm32f767xx_dwt_driver.h"
#include "stm32f767xx_gpio_driver.h"



void DWT_CYCCNT_EN(void)
{
	*DEMCR_BASE_ADDR |= ( 1<< DEMCR_TRACENA );

	*DWT_CYCCNT_BASE_ADDR = 0;

	*DWT_CTRL_BASE_ADDR |= (1 << DWT_CYCCNTENA);

}

void GPIO_LED_INIT(void)
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
}
