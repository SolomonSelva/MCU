/*
 * stm32c031xx_gpio_driver.h
 *
 *  Created on: Nov 7, 2024
 *      Author: solom
 */

#ifndef INC_STM32C031XX_GPIO_DRIVER_H_
#define INC_STM32C031XX_GPIO_DRIVER_H_

#include "stm32c031xx.h"

/* configurable items for User Application */
typedef struct
{
	uint8_t GPIO_PinNumber;			// @GPIO Pin numbers
	uint8_t GPIO_PinMode;			// @GPIO Modes
	uint8_t GPIO_PinSpeed;			// @GPIO output speed
	uint8_t GPIO_PinPuPdControl;	// @GPIO push pull configurations
	uint8_t GPIO_PinOPType;			// @GPIO output types
	uint8_t GPIO_PinAltFuncMode;

}GPIO_PinConfig_t;


/* Handle structure for GPIO */
typedef struct
{
	//Pointer to hold the base address of GPIO peripheral

	GPIO_RegDef_t *pGPIOx;		//This holds the base address of GPIO port to which the PIn belongs
	GPIO_PinConfig_t GPIOPinConfig;
}GPIO_Handle_t;


// @GPIO Pin numbers
#define GPIO_PIN_0					0
#define GPIO_PIN_1					1
#define GPIO_PIN_2					2
#define GPIO_PIN_3					3
#define GPIO_PIN_4					4
#define GPIO_PIN_5					5
#define GPIO_PIN_6					6
#define GPIO_PIN_7					7
#define GPIO_PIN_8					8
#define GPIO_PIN_9					9
#define GPIO_PIN_10					10
#define GPIO_PIN_11					11
#define GPIO_PIN_12					12
#define GPIO_PIN_13					13
#define GPIO_PIN_14					14
#define GPIO_PIN_15					15


// @GPIO Modes
#define GPIO_MODE_IN				0
#define GPIO_MODE_OUT				1
#define GPIO_MODE_AF				2
#define GPIO_MODE_ANALOG			3
// Defined myself
#define GPIO_MODE_IT_FT				4		// Interrupt Falling edge trigger
#define GPIO_MODE_IT_RT				5
#define GPIO_MODE_IT_RFT			3

// @GPIO output types
/* GPIO output type */
#define GPIO_OP_TYPE_PP				0
#define GPIO_OP_TYPE_OD				1

// @GPIO output speed
/* GPIO output speed */
#define GPIO_SPEED_VLOW				0
#define GPIO_SPEED_LOW				1
#define GPIO_SPEED_HIGH				2
#define GPIO_SPEED_VHIGH			3

// @GPIO push pull configurations
/* GPIO Pin Push and Pull configuration */
#define GPIO_NO_PP						0
#define GPIO_PIN_PU						1
#define GPIO_PIN_PD						2

/* Prototype developed for API */
void GPIO_PeriClkControl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi);
void SYSCFG_PeriClkControl (uint8_t EnOrDi);

void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);

void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQ_Priority, uint8_t EnOrDi);
void GPIO_IRQHandling(uint8_t PinNumber);

#endif /* INC_STM32C031XX_GPIO_DRIVER_H_ */
