/*
 * 004_SPI_Tx.c
 *
 *  Created on: Nov 28, 2024
 *      Author: solom
 */

#include <stdio.h>
#include <string.h>
#include "stm32c031xx.h"
#include "stm32c031xx_gpio_driver.h"
#include "stm32c031xx_spi_driver.h"

void delay(void)
{
	for(uint32_t i =0; i<500000/5 ; i++);
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

void SPI_Reception_Func(void)
{
	//char Received_Data[5];
	uint8_t Received_Data;

	/*
	 * Making SSOE 1, makes NSSoutput enable,The NSS pin is automatically managed by hardware.
	 * i.e., when SPE =1, NSS will be 0
	 * when SPE =0, NSS =1
	 */

	SPI_SSOEConfig(SPI1, ENABLE);

	SPI_PeripheralControl(SPI1, ENABLE);

	//Actual Data
	SPI_ReceiveData(SPI1, &Received_Data, 1);

	if (Received_Data == 0x27)
	{
		GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_5, SET);
	}

	// SPI should not be busy before closing or disabling the Peripheral.

	while (SPI_GetFlagStatus(SPI1, SPI_BUSY_FLAG));

	SPI_PeripheralControl(SPI1, DISABLE);
}

void SPI_Transfer_Func ()
{
	//char user_data[] = "Hello";
	uint8_t user_data = 0x27;

	// wait for the button to be pressed
	while ( GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_13));

	delay();

	/*
	 * Making SSOE 1, makes NSSoutput enable,The NSS pin is automatically managed by hardware.
	 * i.e., when SPE =1, NSS will be 0
	 * when SPE =0, NSS =1
	 */

	//SPI_SSOEConfig(SPI1, ENABLE);

	SPI_PeripheralControl(SPI1, ENABLE);

	//Actual Data
	SPI_SendData(SPI1, &user_data, 1);
	//SPI_SendData(SPI1, (uint8_t*)user_data, strlen(user_data));


	// SPI should not be busy before closing or disabling the Peripheral.

	while (SPI_GetFlagStatus(SPI1, SPI_BUSY_FLAG));

	SPI_PeripheralControl(SPI1, DISABLE);
}

int main ()
{

	//uint8_t user_data = 0xff;

	// function to initailaise GPIO pins as SPI1
	SPI1_GpioInit();

	SPI1_Init();

	GPIOButton_Init();

	GPIOLed_Init();

	while (1)
	{
		SPI_Transfer_Func();
		//SPI_Reception_Func();
	}


	return 0;
}
