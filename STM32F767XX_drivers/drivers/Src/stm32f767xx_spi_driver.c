/*
 * stm32f767xx_spi_driver.c
 *
 *  Created on: Nov 21, 2024
 *      Author: solom
 */

#include "stm32f767xx_spi_driver.h"
#include "stm32f767xx.h"

static void spi_txe_handle_interrupt(SPI_Handle_t *pSPIHandle);
static void spi_rxne_handle_interrupt(SPI_Handle_t *pSPIHandle);
static void spi_ovr_handle_interrupt(SPI_Handle_t *pSPIHandle);

void SPI_PeriClkControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if (EnOrDi == ENABLE)
	{
		if (pSPIx == SPI1)
		{
			SPI1_PCLK_EN();
		}
		else if (pSPIx == SPI2)
		{
			SPI2_PCLK_EN();
		}

		else if (pSPIx == SPI3)
		{
			SPI3_PCLK_EN();
		}
		else if (pSPIx == SPI4)
		{
			SPI4_PCLK_EN();
		}
		else if (pSPIx == SPI5)
		{
			SPI5_PCLK_EN();
		}
		else if (pSPIx == SPI6)
		{
			SPI6_PCLK_EN();
		}

		else
		{
			// Do nothing
		}
	}

	else
	{
		if (pSPIx == SPI1)
		{
			SPI1_PCLK_DI();
		}
		else if (pSPIx == SPI2)
		{
			SPI2_PCLK_DI();
		}

		else if (pSPIx == SPI3)
		{
			SPI3_PCLK_DI();
		}
		else if (pSPIx == SPI4)
		{
			SPI4_PCLK_DI();
		}
		else if (pSPIx == SPI5)
		{
			SPI5_PCLK_DI();
		}
		else if (pSPIx == SPI6)
		{
			SPI6_PCLK_DI();
		}

		else
		{
			// Do nothing
		}
	}
}

void SPI_Init(SPI_Handle_t *pSPIHandle)
{

	SPI_PeriClkControl(pSPIHandle->pSPIx, ENABLE);

	uint32_t tempreg = 0;

	// 1. Set the mode
	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPIx_CR1_MSTR;


	// 2. Configure the Bus config

	if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUSCFG_FD)
	{
		// Clear the BIDIMODE bit
		tempreg &= ~(1 << SPIx_CR1_BIDIMODE);
	}
	else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUSCFG_HD)
	{
		// Set the BIDIMODE bit
		tempreg |= (1 << SPIx_CR1_BIDIMODE);

		//Configure BIDIOE bit for direction of transfer??

	}
	else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUSCFG_SIMPLEX_RX)
	{
		// Clear the BIDIMODE bit
		tempreg &= ~(1 << SPIx_CR1_BIDIMODE);

		// RXONLY bit must be set
		tempreg |= (1 << SPIx_CR1_RXONLY);
	}


	// Configure SPI SClock

	tempreg |= (pSPIHandle->SPIConfig.SPI_SclkSpeed << SPIx_CR1_BR);

	// Configure SPI DFF (Data Size DS) is present in CR2

	pSPIHandle->pSPIx->CR2 |= (pSPIHandle->SPIConfig.SPI_DS << SPIx_CR2_DS);

	// Configure SPI CPOL

	tempreg |= (pSPIHandle->SPIConfig.SPI_CPOL << SPIx_CR1_CPOL);

	// Configure SPI CPHA

	tempreg |= (pSPIHandle->SPIConfig.SPI_CPHA << SPIx_CR1_CPHA);

	// Configure SPI SSM

	tempreg |= (pSPIHandle->SPIConfig.SPI_SSM << SPIx_CR1_SSM);


	pSPIHandle->pSPIx->CR1 = tempreg;

}
void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
	if (pSPIx == SPI1)
	{
		SPI1_REG_RST();
	}
	else if (pSPIx == SPI2)
	{
		SPI2_REG_RST();
	}

	else if (pSPIx == SPI3)
	{
		SPI3_REG_RST();
	}
	else if (pSPIx == SPI4)
	{
		SPI4_REG_RST();
	}
	else if (pSPIx == SPI5)
	{
		SPI5_REG_RST();
	}
	else if (pSPIx == SPI6)
	{
		SPI6_REG_RST();
	}
	else
	{
		// DO nothing
	}
}

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName)
{
	if(pSPIx->SR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

uint16_t SPI_GetDataLength(SPI_RegDef_t *pSPIx)
{
	return ((pSPIx->CR2 >> SPIx_CR2_DS) & 15);
}


/* This is a blocking call function as the while loops will be running till a;; the bits are sent */

void SPI_SendData (SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len)
{
	while (Len > 0)
	{
		// 1. wait until TXE is SET
		while (SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);

		// 2. CHeck DFF bit
		if (SPI_GetDataLength(pSPIx) == SPI_DFF_8BITS)
		{
			// 8 bit
			pSPIx->DR = *pTxBuffer;
			Len--;
			pTxBuffer++;
		}
		else if (SPI_GetDataLength(pSPIx) == SPI_DFF_16BITS)
		{
			// 16 bit
			pSPIx->DR = *((uint16_t*)pTxBuffer);
			Len--;
			Len--; // Since 2 bytes of data is sent in 16 bit length data
			(uint16_t*)pTxBuffer++;
		}
	}

}

void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if (EnOrDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPIx_CR1_SPE);
	}

	else
	{
		pSPIx->CR1 &= ~(1 << SPIx_CR1_SPE);
	}
}

void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if (EnOrDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPIx_CR1_SSI);
	}

	else
	{
		pSPIx->CR1 &= ~(1 << SPIx_CR1_SSI);
	}
}

void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if (EnOrDi == ENABLE)
	{
		pSPIx->CR2 |= (1 << SPIx_CR2_SSOE);
	}

	else
	{
		pSPIx->CR2 &= ~(1 << SPIx_CR2_SSOE);
	}
}

void SPI_ReceiveData (SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len)
{
	while (Len > 0)
	{
		// 1. wait until RXNE is SET
		while (SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET);

		// 2. CHeck DFF bit
		if (SPI_GetDataLength(pSPIx) == SPI_DFF_8BITS)
		{
			// 8 bit
			*pRxBuffer = pSPIx->DR;
			Len--;
			pRxBuffer++;
		}
		else if (SPI_GetDataLength(pSPIx) == SPI_DFF_16BITS)
		{
			// 16 bit
			*((uint16_t*)pRxBuffer) = pSPIx->DR;
			Len--;
			Len--; // Since 2 bytes of data is sent in 16 bit length data
			(uint16_t*)pRxBuffer++;
		}
	}
}


void SPI_IRQConfig(uint8_t IRQNumber, uint8_t IRQ_Priority, uint8_t EnOrDi)
{
	// 1. ENABLE or Disable the the interupt in processor(NVIC)
	if (EnOrDi == ENABLE){
		//Check is the provided IRQ number is well within the range (In ths board, we have 7 registers, but considering only till 127 as the IRQ numbers are only till 109)
		if (IRQNumber <= 31)
		{
			// Program ISER0 register
			*NVIC_ISER0_BASE_ADDR |= (1 << IRQNumber);
		}
		else if ((IRQNumber > 31) && (IRQNumber <64)) // IRQ number 32 to 63
		{
			// Program ISER1 register
			*NVIC_ISER1_BASE_ADDR |= (1 << (IRQNumber % 32));
		}
		else if ((IRQNumber >= 64) && (IRQNumber <96)) // IRQ number 64 to 95
		{
			// Program ISER2 register
			*NVIC_ISER2_BASE_ADDR |= (1 << (IRQNumber % 64));
		}
		else if ((IRQNumber >= 96) && (IRQNumber <128)) //IRQ number from 96 to 127
		{
			// Program ISER3 register
			*NVIC_ISER3_BASE_ADDR |= (1 << (IRQNumber % 96));
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
			*NVIC_ICER0_BASE_ADDR |= (1 << IRQNumber);
		}
		else if ((IRQNumber > 31) && (IRQNumber <64)) // IRQ number 32 to 63
		{
			// Program ICER register
			*NVIC_ICER1_BASE_ADDR |= (1 << (IRQNumber % 32));
		}
		else if ((IRQNumber >= 64) && (IRQNumber <96)) // IRQ number 64 to 95
		{
			// Program ICER register
			*NVIC_ICER2_BASE_ADDR |= (1 << (IRQNumber % 64));
		}
		else if ((IRQNumber >= 96) && (IRQNumber <128)) //IRQ number from 96 to 127
		{
			// Program ICER register
			*NVIC_ICER3_BASE_ADDR |= (1 << (IRQNumber % 96));
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

uint8_t SPI_SendDataIT (SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len)
{

	/*
	 * 1. Save Txbuffer and Len info in a global variable
	 *
	 * 2. Till the SPI transmission is happening, make sure no other peripheral calls for the same SPI by setting the Busy flag
	 *
	 * 3. ENable TXEIE to get interupt whenever TXE flag is set in SR
	 *
	 * 4. Data transmission will be handled in the ISR handling.
	 */
	uint8_t spi_state = pSPIHandle->TxState;

	if(spi_state != SPI_TX_BSY)
	{
		pSPIHandle->pTxBuffer = pTxBuffer;

		pSPIHandle->TxLen = Len;

		pSPIHandle->TxState = SPI_TX_BSY;

		pSPIHandle->pSPIx->CR2 |= (1 << SPIx_CR2_TXEIE);

	}

	return spi_state;
}
uint8_t SPI_ReceiveDataIT (SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len)
{
	/*
	 * 1. Save Rxbuffer and Len info in a global variable
	 *
	 * 2. Till the SPI transmission is happening, make sure no other peripheral calls for the same SPI by setting the Busy flag
	 *
	 * 3. ENable RXEIE to get interupt whenever RXE flag is set in SR
	 *
	 * 4. Data transmission will be handled in the ISR handling.
	 */
	uint8_t spi_state = pSPIHandle->TxState;

	if(spi_state != SPI_RX_BSY)
	{
		pSPIHandle->pRxBuffer = pRxBuffer;

		pSPIHandle->RxLen = Len;

		pSPIHandle->RxState = SPI_RX_BSY;

		pSPIHandle->pSPIx->CR2 |= (1 << SPIx_CR2_RXNEIE);

	}

	return spi_state;
}

void SPI_IRQHandling(SPI_Handle_t *pSPIHandle)
{

	uint8_t temp1, temp2;

	// Check for TXE

	temp1 = pSPIHandle->pSPIx->CR2 & (1 << SPIx_CR2_TXEIE);

	temp2 = pSPIHandle->pSPIx->SR & (1 << SPIx_SR_TXE);

	if (temp1 && temp2)
	{
		// Handle TXE
		spi_txe_handle_interrupt(pSPIHandle);
	}

	// Check for RXNE

	temp1 = pSPIHandle->pSPIx->CR2 & (1 << SPIx_CR2_RXNEIE);

	temp2 = pSPIHandle->pSPIx->SR & (1 << SPIx_SR_RXNE);

	if (temp1 && temp2)
	{
		// Handle TXE
		spi_rxne_handle_interrupt(pSPIHandle);
	}

	// Check for OVR

	temp1 = pSPIHandle->pSPIx->CR2 & (1 << SPIx_CR2_ERRIE);

	temp2 = pSPIHandle->pSPIx->SR & (1 << SPIx_SR_OVR);

	if (temp1 && temp2)
	{
		// Handle TXE
		spi_ovr_handle_interrupt(pSPIHandle);
	}

}

static void spi_txe_handle_interrupt(SPI_Handle_t *pSPIHandle)
{
	// 2. CHeck DFF bit
	if (SPI_GetDataLength(pSPIHandle->pSPIx) == SPI_DFF_8BITS)
	{
		// 8 bit
		pSPIHandle->pSPIx->DR = *pSPIHandle->pTxBuffer;
		pSPIHandle->TxLen--;
		pSPIHandle->pTxBuffer++;
	}
	else if (SPI_GetDataLength(pSPIHandle->pSPIx) == SPI_DFF_16BITS)
	{
		// 16 bit
		pSPIHandle->pSPIx->DR = *((uint16_t*)pSPIHandle->pTxBuffer);
		pSPIHandle->TxLen--;
		pSPIHandle->TxLen--; // Since 2 bytes of data is sent in 16 bit length data
		(uint16_t*)pSPIHandle->pTxBuffer++;
	}

	if (! pSPIHandle->TxLen )
	{
		/*
		 * Txlen is 0 means transmission is done and should close the SPI transmission and inform the application
		 * that the Tx is over.
		 */

		//CLearing the TXEIE bit to prevent the interrupt from setting up of TXE flag
		SPICloseTransmission(pSPIHandle);
		SPIApplicationEventCallback(pSPIHandle, SPI_EVENT_TX_CMPLT);

	}
}

static void spi_rxne_handle_interrupt(SPI_Handle_t *pSPIHandle)
{
	// 2. CHeck DFF bit
	if (SPI_GetDataLength(pSPIHandle->pSPIx) == SPI_DFF_8BITS)
	{
		// 8 bit
		*(pSPIHandle->pRxBuffer) = pSPIHandle->pSPIx->DR ;
		pSPIHandle->RxLen--;
		pSPIHandle->pRxBuffer++;
	}
	else if (SPI_GetDataLength(pSPIHandle->pSPIx) == SPI_DFF_16BITS)
	{
		// 16 bit
		*((uint16_t*)pSPIHandle->pRxBuffer) = pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen--;
		pSPIHandle->RxLen--; // Since 2 bytes of data is sent in 16 bit length data
		(uint16_t*)pSPIHandle->pRxBuffer++;
	}

	if (! pSPIHandle->TxLen )
	{
		/*
		 * Txlen is 0 means transmission is done and should close the SPI transmission and inform the application
		 * that the Tx is over.
		 */

		//CLearing the TXEIE bit to prevent the interrupt from setting up of TXE flag

		SPICloseReception(pSPIHandle);
		SPIApplicationEventCallback(pSPIHandle, SPI_EVENT_RX_CMPLT);

	}
}

static void spi_ovr_handle_interrupt(SPI_Handle_t *pSPIHandle)
{
	uint8_t temp;

	if(pSPIHandle->TxState != SPI_TX_BSY)
	{
		temp = pSPIHandle->pSPIx->DR;
		temp = pSPIHandle->pSPIx->SR;

	}
	(void)temp;
	SPIApplicationEventCallback(pSPIHandle, SPI_EVENT_OVR_ERR);

}

void SPICloseTransmission(SPI_Handle_t *pSPIHandle)
{

	pSPIHandle->pSPIx->CR2 &= ~(1 << SPIx_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TxLen = 0;
	pSPIHandle->TxState = SPI_READY;
}

void SPICloseReception(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPIx_CR2_RXNEIE);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxLen = 0;
	pSPIHandle->RxState = SPI_READY;
}


__weak void SPIApplicationEventCallback (SPI_Handle_t *pSPIHandle, uint8_t AppEv)
{
	/*
	 * This is a weak function, which will be written by application
	 */
}
