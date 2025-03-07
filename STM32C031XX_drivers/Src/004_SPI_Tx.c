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
	//SPI_Pins.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_12;
	//GPIO_Init(&SPI_Pins);

	//SPI2_SCK
	SPI_Pins.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_13;
	GPIO_Init(&SPI_Pins);

	//SPI2_MISO
	//SPI_Pins.GPIOPinConfig.GPIO_PinNumber = GPIO_PIN_14;
	//GPIO_Init(&SPI_Pins);

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
	SPI2Handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2;
	SPI2Handle.SPIConfig.SPI_DS = SPI_DFF_8BITS;
	SPI2Handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2Handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2Handle.SPIConfig.SPI_SSM = SPI_SSM_EN; // SW slave management enabled for SPI2

	SPI_Init(&SPI2Handle);
}

int main ()
{

	char user_data[] = "Hello World!";

	// function to initailaise GPIO pins as SPI2
	SPI2_GpioInit();

	SPI2_Init();

	SPI_SSIConfig(SPI2, ENABLE);

	SPI_PeripheralControl(SPI2, ENABLE);

	SPI_SendData(SPI2, (uint8_t*)user_data, strlen(user_data));

	// SPI should not be busy before closing or disabling the Peripheral.

	while (SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));

	SPI_PeripheralControl(SPI2, DISABLE);

	while(1);

	return 0;
}
