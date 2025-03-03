/*
 * 009_USART_TX_Arduino.c
 *
 *  Created on: Feb 4, 2025
 *      Author: solom
 */

#include "stm32f767xx.h"
#include "stm32f767xx_gpio_driver.h"
#include "stm32f767xx_usart_driver.h"
#include "string.h"

char mesg[1024] = "UART Tx Testing .....\n\r";

USART_Handle_t usart3_handle;

//This flag indicates reception completion
uint8_t rxCmplt = RESET;

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i ++);
}


void USART3_Init(void)
{
	usart3_handle.pUSARTx = USART3;
	usart3_handle.USART_Config.USART_Baud = USART_STD_BAUD_115200;
	usart3_handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
	usart3_handle.USART_Config.USART_Mode = USART_MODE_ONLY_TX;
	usart3_handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
	usart3_handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
	usart3_handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
	USART_Init(&usart3_handle);
}


void USART3_GPIOInit (void)
{
	GPIO_Handle_t usart_gpios;

	usart_gpios.pGPIOx = GPIOD;
	usart_gpios.GPIOPinConfig.GPIO_PinAltFuncMode = 7;
	usart_gpios.GPIOPinConfig.GPIO_PinMode = GPIO_MODE_AF;
	usart_gpios.GPIOPinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	usart_gpios.GPIOPinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	usart_gpios.GPIOPinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;

	usart_gpios.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_8;
	GPIO_Init(&usart_gpios);

	usart_gpios.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_9;
	GPIO_Init(&usart_gpios);
}

void GPIO_Button_Init()
{
	GPIO_Handle_t GpioButton;

	GpioButton.pGPIOx = GPIOC;
	/* Button config */

	GpioButton.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_13;

	GpioButton.GPIOPinConfig.GPIO_PinMode = GPIO_MODE_IN;

	GpioButton.GPIOPinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;

	//GpioButton.GPIOPinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;

	GpioButton.GPIOPinConfig.GPIO_PinPuPdControl  = GPIO_NO_PP;

	GPIO_PeriClkControl(GPIOC, ENABLE);

	GPIO_Init(&GpioButton);
}

int main (void)
{
	GPIO_Button_Init();

	USART3_GPIOInit();

	USART3_Init();

	USART_IRQConfig(IRQ_USART3, IRQ_PRIO_46, ENABLE);

	USART_PeripheralControl(USART3, ENABLE);

	while (1)
	{
		while (! GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_13));

		delay();

		USART_SendData(&usart3_handle, (uint8_t*)mesg,strlen(mesg));
	}

	return 0;
}

void USART3_IRQHandler(void)
{
	USART_IRQHandling(&usart3_handle);
}

void USART_ApplicationEventCallback( USART_Handle_t *pUSARTHandle,uint8_t ApEv)
{
   if(ApEv == USART_EVENT_RX_CMPLT)
   {
			rxCmplt = SET;

   }else if (ApEv == USART_EVENT_TX_CMPLT)
   {
	   ;
   }
}
