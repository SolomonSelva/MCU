/*
 * 004_SPI_Tx.c
 *
 *  Created on: Nov 28, 2024
 *      Author: solom
 */

#include <string.h>
#include "stm32c031xx.h"
#include "stm32c031xx_gpio_driver.h"
#include "stm32c031xx_spi_driver.h"


#define CMD_LED_CTRL				0x50
#define CMD_SENSOR_READ				0x51
#define CMD_LED_READ				0x52
#define CMD_PRINT					0x53
#define CMD_ID_READ					0x54


#define ACK							0xF5
#define NACK						0xA5

#define LED_ON						1
#define LED_OFF						0

#define LED_PB7						27

void delay(void)
{
	for(uint32_t i =0; i<500000/2 ; i++);
}

/*
 * WHen AF is 0:
 * PB0 -> SPI1_NSS
 * PB3 -> SPI1_SCK
 * PB4 -> SPI1_MISO
 * PB5 -> SPI1_MOSI
 */


void SPI1_GpioInit(void)
{
	GPIO_Handle_t SPI_Pins;
	SPI_Pins.pGPIOx = GPIOB;
	SPI_Pins.GPIOPinConfig.GPIO_PinMode = GPIO_MODE_AF;
	SPI_Pins.GPIOPinConfig.GPIO_PinAltFuncMode = 0;
	SPI_Pins.GPIOPinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPI_Pins.GPIOPinConfig.GPIO_PinPuPdControl = GPIO_NO_PP;

	// SPI1_NSS
	SPI_Pins.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_0;
	GPIO_Init(&SPI_Pins);

	//SPI1_SCK
	SPI_Pins.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_3;
	GPIO_Init(&SPI_Pins);

	//SPI1_MISO
	SPI_Pins.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_4;
	GPIO_Init(&SPI_Pins);

	//SPI1_MOSI

	SPI_Pins.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_5;
	GPIO_Init(&SPI_Pins);

}

void SPI1_Init(void)
{
	SPI_Handle_t SPI1Handle;

	SPI1Handle.pSPIx = SPI1;
	SPI1Handle.SPIConfig.SPI_BusConfig = SPI_BUSCFG_FD;
	SPI1Handle.SPIConfig.SPI_DeviceMode = SPI_DEVICEMODE_SLAVE;
	SPI1Handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8;
	SPI1Handle.SPIConfig.SPI_DS = SPI_DFF_8BITS;
	SPI1Handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI1Handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI1Handle.SPIConfig.SPI_SSM = SPI_SSM_DI; // SW slave management enabled for SPI1

	SPI_Init(&SPI1Handle);
}

void GPIOButton_Init()
{
	/* Button config */
	GPIO_Handle_t GpioButton;

	GpioButton.pGPIOx = GPIOC;

	GpioButton.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_13;

	GpioButton.GPIOPinConfig.GPIO_PinMode = GPIO_MODE_IN;

	GpioButton.GPIOPinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;

	//GpioButton.GPIOPinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;

	GpioButton.GPIOPinConfig.GPIO_PinPuPdControl  = GPIO_NO_PP;

	GPIO_Init(&GpioButton);
}

void GPIOLed_Init()
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
}


int main ()
{
	uint8_t dummy_write = 0xff;
	uint8_t dummy_read;

	// function to initailaise GPIO pins as SPI1
	SPI1_GpioInit();

	SPI1_Init();

	GPIOLed_Init();

	while (1)
	{
		/*
		 * Making SSOE 1, makes NSSoutput enable,The NSS pin is automatically managed by hardware.
		 * i.e., when SPE =1, NSS will be 0
		 * when SPE =0, NSS =1
		 */

		SPI_SSOEConfig(SPI1, ENABLE);

		SPI_PeripheralControl(SPI1, ENABLE);

		uint8_t commandcode;
		uint8_t args;

		//Actual Data
		SPI_ReceiveData(SPI1, &commandcode, 1);

		printf("Received data is %x\n", commandcode);

		if (commandcode == CMD_LED_CTRL)
		{
			SPI_SendData(SPI1, (uint8_t*)ACK, 1);
			printf("Sending ACK to Master\n");
		}
		else
		{
			SPI_SendData(SPI1, (uint8_t*)NACK, 1);
			printf("Sending NACK to Master\n");
		}

		SPI_ReceiveData(SPI1, &dummy_read, 1);

		SPI_ReceiveData(SPI1, &args, 1);

		if (args == LED_PB7)
		{
			GPIO_WriteToOutputPin(GPIOB, 7, 1);
		}
		else
		{
			GPIO_WriteToOutputPin(GPIOB, 7, 0);
		}

		// SPI should not be busy before closing or disabling the Peripheral.

		while (SPI_GetFlagStatus(SPI1, SPI_BUSY_FLAG));

		SPI_PeripheralControl(SPI1, DISABLE);
	}


	return 0;
}
