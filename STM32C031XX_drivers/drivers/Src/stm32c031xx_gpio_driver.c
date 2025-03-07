/*
 * stm32c031xx_gpio_driver.c
 *
 *  Created on: Nov 7, 2024
 *      Author: solom
 */

#include "stm32c031xx_gpio_driver.h"

void GPIO_PeriClkControl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi)
{
	if (EnOrDi == ENABLE)
	{
		if (pGPIOx == GPIOA)
		{
			GPIOA_PCLK_EN();
		}
		else if (pGPIOx == GPIOB)
		{
			GPIOB_PCLK_EN();
		}

		else if (pGPIOx == GPIOC)
		{
			GPIOC_PCLK_EN();
		}
		else if (pGPIOx == GPIOD)
		{
			GPIOD_PCLK_EN();
		}
		else if (pGPIOx == GPIOF)
		{
			GPIOF_PCLK_EN();
		}
		else
		{
			// Do nothing
		}
	}

	else
	{
		if (pGPIOx == GPIOA)
		{
			GPIOA_PCLK_DI();
		}
		else if (pGPIOx == GPIOB)
		{
			GPIOB_PCLK_DI();
		}

		else if (pGPIOx == GPIOC)
		{
			GPIOC_PCLK_DI();
		}
		else if (pGPIOx == GPIOD)
		{
			GPIOD_PCLK_DI();
		}
		else if (pGPIOx == GPIOF)
		{
			GPIOF_PCLK_DI();
		}
		else
		{
			// Do nothing
		}
	}
}

void SYSCFG_PeriClkControl (uint8_t EnOrDi)
{
	if (EnOrDi == ENABLE)
	{
		SYSCFG_PCLK_EN();
	}
	else
	{
		SYSCFG_PCLK_DI();
	}
}

void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	// Enable the peripheral clock, so that it is not required by the user to manually write it.
	GPIO_PeriClkControl(pGPIOHandle->pGPIOx, ENABLE);

	uint32_t temp = 0; //temporary register

	//1. Configure the mode of GPIO pin

	if (pGPIOHandle->GPIOPinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{
		// Non interrupt mode
		temp = (pGPIOHandle-> GPIOPinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIOPinConfig.GPIO_PinNumber) );
		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << (2* pGPIOHandle->GPIOPinConfig.GPIO_PinNumber)); // Clearing the required bits to avoid manipulation while setting them below.
		pGPIOHandle->pGPIOx->MODER |= temp;

	}

	else
	{
		// Whenever the Pin is in Interupt mode, it requires some input, hence the pin set to input mode .

		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << (2* pGPIOHandle->GPIOPinConfig.GPIO_PinNumber));

		// Non interrupt mode
		if (pGPIOHandle->GPIOPinConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
		{
			// Configure the FTSR

			EXTI->EXTI_FTSR1 |= ( 1 << pGPIOHandle->GPIOPinConfig.GPIO_PinNumber);

			// clear RTSR to be sure
			EXTI->EXTI_RTSR1 &= ~( 1 << pGPIOHandle->GPIOPinConfig.GPIO_PinNumber);
		}
		else if (pGPIOHandle->GPIOPinConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
		{
			// Configure the RTSR
			EXTI->EXTI_RTSR1 |= ( 1 << pGPIOHandle->GPIOPinConfig.GPIO_PinNumber);

			// clear FTSR to be sure
			EXTI->EXTI_FTSR1 &= ~( 1 << pGPIOHandle->GPIOPinConfig.GPIO_PinNumber);
		}
		else if (pGPIOHandle->GPIOPinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT)
		{
			// Configure both FTSR and RTSR
			EXTI->EXTI_FTSR1 |= ( 1 << pGPIOHandle->GPIOPinConfig.GPIO_PinNumber);

			// Set RTSR as well
			EXTI->EXTI_RTSR1 |= ( 1 << pGPIOHandle->GPIOPinConfig.GPIO_PinNumber);
		}

		// COnfigure the GPIO port selection EXTI_EXTICR (This register is available in EXTI register itself unlike in the course )

		uint8_t temp1 = pGPIOHandle->GPIOPinConfig.GPIO_PinNumber / 4;
		uint8_t temp2 = pGPIOHandle->GPIOPinConfig.GPIO_PinNumber % 4;
		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle ->pGPIOx);

		EXTI->EXTI_EXTICR[temp1] |= (portcode << (8 * temp2) );

		// Enable the EXTI interupt delivery using IMR
		EXTI->EXTI_IMR1 |= (1 << pGPIOHandle->GPIOPinConfig.GPIO_PinNumber);
	}
	temp = 0;

	//2. Configure the speed of GPIO pin

	temp = (pGPIOHandle-> GPIOPinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->GPIOPinConfig.GPIO_PinNumber) );
	pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << (2 * pGPIOHandle->GPIOPinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDR |= temp;
	temp = 0;

	//3. Configure the pupd of GPIO pin
	temp = (pGPIOHandle-> GPIOPinConfig.GPIO_PinPuPdControl << (2 * pGPIOHandle->GPIOPinConfig.GPIO_PinNumber) );
	pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << (2 * pGPIOHandle->GPIOPinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR |= temp;
	temp = 0;

	//4. Configure the optype of GPIO pin
	temp = (pGPIOHandle-> GPIOPinConfig.GPIO_PinOPType << (pGPIOHandle->GPIOPinConfig.GPIO_PinNumber) );
	pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << pGPIOHandle->GPIOPinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OTYPER |= temp;
	temp = 0;

	//5. Configure the Alternate functionality of GPIO pin
	if (pGPIOHandle->GPIOPinConfig.GPIO_PinMode == GPIO_MODE_AF)
	{
		// if the mode if ALternate function, only then configure AF.

		uint32_t temp1 = pGPIOHandle-> GPIOPinConfig.GPIO_PinNumber % 8 ;

		if (pGPIOHandle-> GPIOPinConfig.GPIO_PinNumber < 8 )
		{
			pGPIOHandle->pGPIOx->AFRL &= ~(0xf << (4 * temp1));
			pGPIOHandle->pGPIOx->AFRL |= (pGPIOHandle-> GPIOPinConfig.GPIO_PinAltFuncMode << (4 * temp1));
		}
		else
		{
			pGPIOHandle->pGPIOx->AFRH &= ~(0xf << (4 * temp1));
			pGPIOHandle->pGPIOx->AFRH |= (pGPIOHandle-> GPIOPinConfig.GPIO_PinAltFuncMode << (4 * temp1));
		}
	}

}
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
	if (pGPIOx == GPIOA)
	{
		GPIOA_REG_RST();
	}
	else if (pGPIOx == GPIOB)
	{
		GPIOB_REG_RST();
	}

	else if (pGPIOx == GPIOC)
	{
		GPIOC_REG_RST();
	}
	else if (pGPIOx == GPIOD)
	{
		GPIOD_REG_RST();
	}
	else if (pGPIOx == GPIOF)
	{
		GPIOF_REG_RST();
	}
	else
	{
		// Do nothing
	}

}

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	uint8_t value;
	value = (uint8_t) ((pGPIOx->IDR >> PinNumber) & 0x00000001 );
	return value;

}
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t value;
	value = (uint16_t) (pGPIOx->IDR) ;
	return value;
}

void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value)
{
	if (Value == GPIO_PIN_SET)
	{
		// Set the pin of the corresponding bit field of the port
		pGPIOx->ODR |= (1 << PinNumber);
	}
	else
	{
		//Clear the pin
		pGPIOx->ODR &= ~(1 << PinNumber);
	}

}
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value)
{

	Value = pGPIOx->ODR;
}
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	pGPIOx->ODR ^= (1 << PinNumber);
}

void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQ_Priority, uint8_t EnOrDi)
{
	// 1. ENABLE or Disable the the interupt in processor(NVIC)
	if (EnOrDi == ENABLE){
		//Check is the provided IRQ number is well within the range (In ths board, we have only 1 register)
		if (IRQNumber <= 31)
		{
			// Program ISER register
			*NVIC_ISER_BASE_ADDR |= (1 << IRQNumber);
		}
		else{
			//Do nothing
		}
	}
	else{
		//Check is the provided IRQ number is well within the range (In ths board, we have only 1 register)
		if (IRQNumber <= 31)
		{
			// Program ICER register
			*NVIC_ICER_BASE_ADDR |= (1 << IRQNumber);
		}
		else{
			//Do nothing
		}
	}

	//2. Set the priority for each IRQNumber

	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;
	uint8_t shift_amount = (iprx_section * 8) + (8 - NO_PR_BITS_IMPLEMENTED);

	*(NVIC_IPR_BASE_ADDR + iprx) |= (IRQ_Priority << shift_amount); // When dealing with pointers +1 will actually add +1*data type


}
void GPIO_IRQHandling(uint8_t PinNumber)
{
	//Clear the EXTI pending register
	if (EXTI->EXTI_FPR1 & (1 << PinNumber))
	{
		EXTI-> EXTI_FPR1 |= (1<< PinNumber);
	}

	//Clear the EXTI pending register
	if (EXTI->EXTI_RPR1 & (1 << PinNumber))
	{
		EXTI-> EXTI_RPR1 |= (1<< PinNumber);
	}

}
