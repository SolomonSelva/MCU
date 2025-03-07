/*
 * 004_SPI_Tx.c
 *
 *  Created on: Nov 28, 2024
 *      Author: solom
 */
#include <stdio.h>
#include <string.h>
#include "stm32f767xx.h"
#include "stm32f767xx_gpio_driver.h"
#include "stm32f767xx_spi_driver.h"

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
	for(uint32_t i =0; i<500000/5 ; i++);
}

/*
 * WHen AF is 5:
 * PB12 -> SPI2_NSS
 * PB13 -> SPI2_SCK
 * PB14 -> SPI2_MISO
 * PB15 -> SPI2_MOSI
 */


void SPI2_GpioInit(void)
{
	GPIO_Handle_t SPI_Pins;
	SPI_Pins.pGPIOx = GPIOB;
	SPI_Pins.GPIOPinConfig.GPIO_PinMode = GPIO_MODE_AF;
	SPI_Pins.GPIOPinConfig.GPIO_PinAltFuncMode = 5;
	SPI_Pins.GPIOPinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPI_Pins.GPIOPinConfig.GPIO_PinPuPdControl = GPIO_NO_PP;

	// SPI2_NSS
	SPI_Pins.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_12;
	GPIO_Init(&SPI_Pins);

	//SPI2_SCK
	SPI_Pins.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_13;
	GPIO_Init(&SPI_Pins);

	//SPI2_MISO
	SPI_Pins.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_14;
	GPIO_Init(&SPI_Pins);

	//SPI2_MOSI

	SPI_Pins.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_15;
	GPIO_Init(&SPI_Pins);

}

void SPI2_Init(void)
{
	SPI_Handle_t SPI2Handle;

	SPI2Handle.pSPIx = SPI2;
	SPI2Handle.SPIConfig.SPI_BusConfig = SPI_BUSCFG_FD;
	SPI2Handle.SPIConfig.SPI_DeviceMode = SPI_DEVICEMODE_MASTER;
	SPI2Handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8;
	SPI2Handle.SPIConfig.SPI_DS = SPI_DFF_8BITS;
	SPI2Handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2Handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2Handle.SPIConfig.SPI_SSM = SPI_SSM_DI; // HW slave management enabled for SPI2

	SPI_Init(&SPI2Handle);
}

void GPIOLed_Init()
{
	/* Button config */
	GPIO_Handle_t GpioLed;

	GpioLed.pGPIOx = GPIOB;

	/* LED config */

	GpioLed.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_7;

	GpioLed.GPIOPinConfig.GPIO_PinMode = GPIO_MODE_OUT;

	GpioLed.GPIOPinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;

	GpioLed.GPIOPinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;

	GpioLed.GPIOPinConfig.GPIO_PinPuPdControl  = GPIO_NO_PP;

	GPIO_Init(&GpioLed);
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


uint8_t SPI_VerifyAckByte (uint8_t ackbyte)
{
	if (ackbyte == 0xF5)
	{
		return 1;
	}
	return 0;
}

int main ()
{

	//uint8_t Received_Data;

	uint8_t dummy_write = 0xff;
	uint8_t dummy_read;

	// function to initailaise GPIO pins as SPI2
	SPI2_GpioInit();

	SPI2_Init();

	GPIOButton_Init();

	while (1)
	{
		// wait for the button to be pressed
		while (! GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_13));

		delay();

		/*
		 * Making SSOE 1, makes NSSoutput enable,The NSS pin is automatically managed by hardware.
		 * i.e., when SPE =1, NSS will be 0
		 * when SPE =0, NSS =1
		 */

		SPI_SSOEConfig(SPI2, ENABLE);

		SPI_PeripheralControl(SPI2, ENABLE);
		uint8_t ackbyte;

		uint8_t commandcode = CMD_LED_CTRL;

		uint8_t args;

		//Actual Data
		SPI_SendData(SPI2, &commandcode, 1);

		// DUmmy read
		SPI_ReceiveData(SPI2, &dummy_read, 1);

		//Send Dummy Data
		SPI_SendData(SPI2, &dummy_write, 1);

		//Receive ACK byte
		SPI_ReceiveData(SPI2, &ackbyte, 1);

		if (SPI_VerifyAckByte(ackbyte))
		{
			// Send arguments
			args = LED_PB7;
			SPI_SendData(SPI2, &args, 1);
		}


		// SPI should not be busy before closing or disabling the Peripheral.

		while (SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));

		SPI_PeripheralControl(SPI2, DISABLE);

	}

	return 0;
}
