/*
 * stm32f767xx_spi_driver.h
 *
 *  Created on: Nov 21, 2024
 *      Author: solom
 */

#ifndef INC_STM32F767XX_SPI_DRIVER_H_
#define INC_STM32F767XX_SPI_DRIVER_H_

#include "stm32f767xx.h"

/* configurable items for User Application */

typedef struct
{
	uint8_t SPI_DeviceMode;
	uint8_t SPI_BusConfig;
	uint8_t SPI_SclkSpeed;
	uint8_t SPI_DS;
	uint8_t SPI_CPOL;
	uint8_t SPI_CPHA;
	uint8_t SPI_SSM;

}SPI_Config_t;

/* Handle structure for SPI */

typedef struct
{
	SPI_RegDef_t *pSPIx;
	SPI_Config_t SPIConfig;
	uint8_t *pTxBuffer;
	uint8_t *pRxBuffer;
	uint32_t TxLen;
	uint32_t RxLen;
	uint8_t TxState;
	uint8_t RxState;

}SPI_Handle_t;

/*
 * SPI Status
 */

#define SPI_READY								0
#define SPI_TX_BSY								1
#define SPI_RX_BSY								2

/*
 * @SPI_DeviceMode
 */

#define SPI_DEVICEMODE_MASTER					1
#define SPI_DEVICEMODE_SLAVE 					0

/*
 * @SPI_BusConfig
 */

#define SPI_BUSCFG_FD							1
#define SPI_BUSCFG_HD							2
#define SPI_BUSCFG_SIMPLEX_RX					3

/*
 * @SPI_SclkSpeed
 */


#define SPI_SCLK_SPEED_DIV2						0
#define SPI_SCLK_SPEED_DIV4						1
#define SPI_SCLK_SPEED_DIV8						2
#define SPI_SCLK_SPEED_DIV16					3
#define SPI_SCLK_SPEED_DIV32					4
#define SPI_SCLK_SPEED_DIV64					5
#define SPI_SCLK_SPEED_DIV128					6
#define SPI_SCLK_SPEED_DIV256					7

/*
 * @SPI_DFF
 * In the STM32F767ZI MCU there are more options to set the data length(refer RM) butI will be using only 8 or 16 bits length
 */
#define SPI_DFF_8BITS							7
#define SPI_DFF_16BITS							15


/*
 * @SPI_CPOL
 */

#define SPI_CPOL_HIGH							1
#define SPI_CPOL_LOW							0


/*
 * @SPI_CPHA
 */

#define SPI_CPHA_HIGH							1
#define SPI_CPHA_LOW							0


/*
 * @SPI_CPHA
 */

#define SPI_SSM_DI								0
#define SPI_SSM_EN								1

#define SPI_TXE_FLAG							(1 << SPIx_SR_TXE)
#define SPI_RXNE_FLAG							(1 << SPIx_SR_RXNE)
#define SPI_BUSY_FLAG							(1 << SPIx_SR_BSY)


/*
 * SPI possible event
 */

#define SPI_EVENT_TX_CMPLT						1
#define SPI_EVENT_RX_CMPLT						2
#define SPI_EVENT_OVR_ERR						3
#define SPI_EVENT_CRC_ERR						4

/* Prototype developed for API */
void SPI_PeriClkControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);

void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

void SPI_SendData (SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);
void SPI_ReceiveData (SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);

uint8_t SPI_SendDataIT (SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len);
uint8_t SPI_ReceiveDataIT (SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len);


void SPI_IRQConfig(uint8_t IRQNumber, uint8_t IRQ_Priority, uint8_t EnOrDi);
void SPI_IRQHandling(SPI_Handle_t *pSPIHandle);

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName);
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);

void SPICloseReception(SPI_Handle_t *pSPIHandle);
void SPICloseTransmission(SPI_Handle_t *pSPIHandle);

/*
 * APplication Callback
 */

void SPIApplicationEventCallback (SPI_Handle_t *pSPIHandle, uint8_t AppEv);



#endif /* INC_STM32F767XX_SPI_DRIVER_H_ */