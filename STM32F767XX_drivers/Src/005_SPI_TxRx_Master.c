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
	SPI2Handle.SPIConfig.SPI_SSM = SPI_SSM_DI; // SW slave management enabled for SPI2

	SPI_Init(&SPI2Handle);
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

void SPI_Transfer_Func ()
{
	//char user_data[] = "Hello";
	uint8_t user_data = 0x27;

	// wait for the button to be pressed
	while ( ! GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_13));

	delay();

	/*
	 * Making SSOE 1, makes NSSoutput enable,The NSS pin is automatically managed by hardware.
	 * i.e., when SPE =1, NSS will be 0
	 * when SPE =0, NSS =1
	 */

	SPI_SSOEConfig(SPI2, ENABLE);

	SPI_PeripheralControl(SPI2, ENABLE);

	/*
	 * Arduino expects 1 byte of data , informing about the length of data thta will be received on SPI.
	 * But need to confirm , if i owuld need this for receiving on STM itself
	 */

	//uint8_t DataLen = strlen(user_data);

	//SPI_SendData(SPI2, &DataLen, 1);

	//Actual Data
	SPI_SendData(SPI2, &user_data, 1);
	//SPI_SendData(SPI2, (uint8_t*)user_data, strlen(user_data));


	// SPI should not be busy before closing or disabling the Peripheral.

	while (SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));

	SPI_PeripheralControl(SPI2, DISABLE);
}

void SPI_Reception_Func(void)
{
	//char Received_Data[5];
	uint8_t Received_Data;

	uint8_t dummy_write = 0xff;

	/*
	 * Making SSOE 1, makes NSSoutput enable,The NSS pin is automatically managed by hardware.
	 * i.e., when SPE =1, NSS will be 0
	 * when SPE =0, NSS =1
	 */

	SPI_SSOEConfig(SPI2, ENABLE);

	SPI_PeripheralControl(SPI2, ENABLE);

	/*
	 * In SPI, SLaves cannot send messages on its own. As the master controls the clock and NSS. So to receive data from
	 * slave, master has to send dummy bits(1byte) to receive the data.
	 */
	SPI_SendData(SPI2, &dummy_write, 1);

	//Actual Data
	SPI_ReceiveData(SPI2, &Received_Data, 1);

//	for (int i=0;i<5;i++)
//	{
//		printf("Received data is %d\n", Received_Data[i]);
//	}
	if (Received_Data == 0x27)
	{
		GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_7, SET);
	}


	// SPI should not be busy before closing or disabling the Peripheral.

	while (SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));

	SPI_PeripheralControl(SPI2, DISABLE);
}

int main ()
{

	// function to initailaise GPIO pins as SPI2
	SPI2_GpioInit();

	SPI2_Init();

	GPIOButton_Init();

	GPIOLed_Init();

	while (1)
	{
		SPI_Reception_Func();
		//SPI_Transfer_Func();
	}


	return 0;
}

