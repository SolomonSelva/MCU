/*
 * stm32c031xx.h
 *
 *  Created on: Nov 5, 2024
 *      Author: solom
 */

#ifndef INC_STM32C031XX_H_
#define INC_STM32C031XX_H_

#include <stdint.h>


#define __vo volatile


/* Processor specific addresses */
/* Arm cortex M0+ processor for STM32C031xx */

#define NVIC_ISER_BASE_ADDR							((__vo uint32_t *) 0xE000E100 )

#define NVIC_ICER_BASE_ADDR							((__vo uint32_t *) 0xE000E180 )

#define NVIC_IPR_BASE_ADDR							((__vo uint32_t *) 0xE000E400 )

#define NO_PR_BITS_IMPLEMENTED						2







/* end of Processor specific addresses */

/* Memory base addresses */

#define FLASH_BASEADDR						0x08000000U		// Base address of writable Flash memory(Main Flash)
#define SRAM1_BASEADDR						0x20000000U		// Base address of SRAM base
//#define SRAM2_BASEADDR
#define ROM									0x1FFF0000U    // System Memory is ROM
#define SRAM								SRAM1		// Define SRAM1 as SRAM Base address

/* Base addresses of Bus peripherals */

#define PERIPH_BASEADDR 					0x40000000U	// APB peripheral base address
#define APB1PERIPH_BASEADDR					PERIPH_BASEADDR		// Same as peripheral base
#define APB2PERIPH_BASEADDR					0x40010000U	// Technically, there is only 1 APB bus. Divided into 2 here just for ease of defining.
#define AHBPERIPH_BASEADDR					0x40020000U	// AHB peripheral base address
#define IOPORT_BASEADDR						0x50000000U 	// In this MC, GPIO are connected directly to MP, Check RM.


/* Base address of peripheral hanging on IO Port */


#define GPIOA_BASEADDR						(IOPORT_BASEADDR + 0x0000)
#define GPIOB_BASEADDR						(IOPORT_BASEADDR + 0x0400)
#define GPIOC_BASEADDR						(IOPORT_BASEADDR + 0x0800)
#define GPIOD_BASEADDR						(IOPORT_BASEADDR + 0x0C00)
#define GPIOF_BASEADDR						(IOPORT_BASEADDR + 0x1400)

/* Base address of peripherals hanging on AHB Bus */

#define EXTI_BASEADDR						(AHBPERIPH_BASEADDR + 0x1800)
#define RCC_BASEADDR						(AHBPERIPH_BASEADDR + 0x1000)

/* Base address of peripherals hanging on APB Bus */

/* Addresses with 0x4000 xxxx are termed as APB1 and Addresses with 0x4001 xxxx are termed as APB2,
 * But infact there is only 1 APB bus
 */

#define I2C1_BASEADDR						(APB1PERIPH_BASEADDR + 0x5400)
#define I2C2_BASEADDR						(APB1PERIPH_BASEADDR + 0x5800)

#define SPI1_BASEADDR						(APB2PERIPH_BASEADDR + 0x3000)
#define SPI2_BASEADDR						(APB1PERIPH_BASEADDR + 0x3000) 	// SPI2S1 is same as SPI2

#define USART1_BASEADDR						(APB2PERIPH_BASEADDR + 0x3800)
#define USART2_BASEADDR						(APB1PERIPH_BASEADDR + 0x4400)

#define SYSCFG_BASEADDR						(APB2PERIPH_BASEADDR + 0x0000)



/* Peripheral Register definition structures */

typedef struct
{
	__vo uint32_t MODER;				//GPIO port mode register 				Address offset = 0x00
	__vo uint32_t OTYPER;				//GPIO port output type register		Address offset = 0x04
	__vo uint32_t OSPEEDR;				//GPIO port output speed register
	__vo uint32_t PUPDR;				//GPIO port pull-up/pull-down register
	__vo uint32_t IDR;					//GPIO port input data register
	__vo uint32_t ODR;					//GPIO port output data register
	__vo uint32_t BSRR;					//GPIO port bit set/reset register
	__vo uint32_t LCKR;					//GPIO port configuration lock register
	__vo uint32_t AFRL;					//GPIO alternate function low register
	__vo uint32_t AFRH;					//GPIO alternate function high register
	__vo uint32_t BRR;					//GPIO port bit reset register			Address offset = 0x28 to 0x2C

}GPIO_RegDef_t;

// Peripheral definitions

//GPIO_RegDef_t *pGPIOA = (GPIO_RegDef_t*) GPIOA_BASEADDR;   // This line can be written in a better way as shown below

#define GPIOA 							((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB 							((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC 							((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD 							((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOF 							((GPIO_RegDef_t*)GPIOF_BASEADDR)

//GPIO_RegDef_t *pGPIOA = GPIOA;

#define RCC 							((RCC_RegDef_t*)RCC_BASEADDR)

#define EXTI							((EXTI_RegDef_t*)EXTI_BASEADDR)

typedef struct
{
	__vo uint32_t CR;				//Address offset = 0x00
	__vo uint32_t ICSCR;				//Address offset = 0x04
	__vo uint32_t CFGR;				//
	uint32_t Reserved0[2];				//
	__vo uint32_t CRRCR;					//
	__vo uint32_t CIER;					//
	__vo uint32_t CIFR;					//
	__vo uint32_t CICR;					//
	__vo uint32_t IOPRSTR;					//
	__vo uint32_t AHBRSTR;					//
	__vo uint32_t APBRSTR1;
	__vo uint32_t APBRSTR2;
	__vo uint32_t IOPENR;
	__vo uint32_t AHBENR;
	__vo uint32_t APBENR1;
	__vo uint32_t APBENR2;
	__vo uint32_t IOPSMENR;
	__vo uint32_t AHBSMENR;
	__vo uint32_t APBSMENR1;
	__vo uint32_t APBSMENR2;
	__vo uint32_t CCIPR;
	__vo uint32_t CCIPR2;
	__vo uint32_t CSR1;
	__vo uint32_t CSR2;						//Address offset = 0x60 to 0x64

}RCC_RegDef_t;

/* Peripheral regsiter structure of EXTI */

typedef struct
{
	__vo uint32_t EXTI_RTSR1;				// 				Address offset = 0x000
	__vo uint32_t EXTI_FTSR1;				//		Address offset = 0x04
	__vo uint32_t EXTI_SWIER1;				//
	__vo uint32_t EXTI_RPR1;				//
	__vo uint32_t EXTI_FPR1;					//				//Reserved as this corresponding pin is not available in my board
	uint32_t Reserved1[19];				//		Address offset =
	__vo uint32_t EXTI_EXTICR[4];				// 4 EXTICRx registers
	uint32_t Reserved2[4];					//
	__vo uint32_t EXTI_IMR1;					//
	__vo uint32_t EXTI_EMR1;					//					//Reserved as this corresponding pin is not available in my board			Address offset = 0x094 to 0x098

}EXTI_RegDef_t;

/* Peripheral regsiter structure of SYSCFG */

typedef struct
{
	__vo uint32_t SYSCFG_CFGR1;				// 				Address offset = 0x00
	__vo uint32_t Reserved0[4];				//		Address offset = 0x04 to 0x17
	__vo uint32_t SYSCFG_CFGR2;				//
	__vo uint32_t Reserved1[7];				//Address offset = 0x04 to 0x17
	__vo uint32_t SYSCFG_CFGR3;					//
	__vo uint32_t Reserved2[15];					//Address offset = 0x1C to 0x3B
	__vo uint32_t SYSCFG_ITLINE[16];					//
	uint32_t Reserved3;						//
	__vo uint32_t SYSCFG_ITLINE19;					//
	__vo uint32_t SYSCFG_ITLINE20;					//
	__vo uint32_t SYSCFG_ITLINE21;					//
	__vo uint32_t SYSCFG_ITLINE22;					//
	__vo uint32_t SYSCFG_ITLINE23;					//
	__vo uint32_t SYSCFG_ITLINE24;					//
	__vo uint32_t SYSCFG_ITLINE25;
	__vo uint32_t SYSCFG_ITLINE26;
	__vo uint32_t SYSCFG_ITLINE27;
	__vo uint32_t SYSCFG_ITLINE28;					// Address offset = 0xF0 to 0xF4


}SYSCFG_RegDef_t;

/* Peripheral regsiter structure of SPI */
typedef struct
{
	__vo uint32_t CR1;
	__vo uint32_t CR2;
	__vo uint32_t SR;
	__vo uint32_t DR;
	__vo uint32_t CRCPR;
	__vo uint32_t RXCRCR;
	__vo uint32_t TXCRCR;
	__vo uint32_t I2SCFGR;
	__vo uint32_t I2SPR;

}SPI_RegDef_t;

#define SPI1							((SPI_RegDef_t*)SPI1_BASEADDR)
#define SPI2							((SPI_RegDef_t*)SPI2_BASEADDR)



/* CLock enable macros for GPIO peripherals */

#define GPIOA_PCLK_EN()					(RCC->IOPENR |= (1 << 0))
#define GPIOB_PCLK_EN()					(RCC->IOPENR |= (1 << 1))
#define GPIOC_PCLK_EN()					(RCC->IOPENR |= (1 << 2))
#define GPIOD_PCLK_EN()					(RCC->IOPENR |= (1 << 3))
#define GPIOF_PCLK_EN()					(RCC->IOPENR |= (1 << 5))

/* CLock enable macros for I2Cx peripherals */

#define I2C1_PCLK_EN()					(RCC->APBENR1 |= (1 << 21))
#define I2C2_PCLK_EN()					(RCC->APBENR1 |= (1 << 22))

/* CLock enable macros for SPIx peripherals */

#define SPI1_PCLK_EN()					(RCC->APBENR2 |= (1 << 12))
#define SPI2_PCLK_EN()					(RCC->APBENR1 |= (1 << 14))

/* CLock enable macros for USARTx peripherals */

#define USART1_PCLK_EN()					(RCC->APBENR2 |= (1 << 14))
#define USART2_PCLK_EN()					(RCC->APBENR1 |= (1 << 17))

/* CLock enable macros for SYSCFG peripherals */

#define SYSCFG_PCLK_EN()					(RCC->APBENR2 |= (1 << 0))

/* CLock disable macros for GPIO peripherals */

#define GPIOA_PCLK_DI()					(RCC->IOPENR &= ~(1 << 0))
#define GPIOB_PCLK_DI()					(RCC->IOPENR &= ~(1 << 1))
#define GPIOC_PCLK_DI()					(RCC->IOPENR &= ~(1 << 2))
#define GPIOD_PCLK_DI()					(RCC->IOPENR &= ~(1 << 3))
#define GPIOF_PCLK_DI()					(RCC->IOPENR &= ~(1 << 5))

/* CLock disable macros for I2Cx peripherals */

#define I2C1_PCLK_DI()					(RCC->APBENR1 &= ~(1 << 21))
#define I2C2_PCLK_DI()					(RCC->APBENR1 &= ~(1 << 22))

/* CLock disable macros for SPIx peripherals */

#define SPI1_PCLK_DI()					(RCC->APBENR2 &= ~(1 << 12))
#define SPI2_PCLK_DI()					(RCC->APBENR1 &= ~(1 << 14))

/* CLock disable macros for USARTx peripherals */

#define USART1_PCLK_DI()					(RCC->APBENR2 &= ~(1 << 14))
#define USART2_PCLK_DI()					(RCC->APBENR1 &= ~(1 << 17))

/* CLock disable macros for SYSCFG peripherals */

#define SYSCFG_PCLK_DI()					(RCC->APBENR2 &= ~(1 << 0))


/* GPIO Port Reset macros */

#define GPIOA_REG_RST()					do { (RCC->IOPRSTR |= (1 << 0));	(RCC->IOPRSTR &= ~(1 << 0));} while (0) // setting and resetting the bit.
#define GPIOB_REG_RST()					do { (RCC->IOPRSTR |= (1 << 1));	(RCC->IOPRSTR &= ~(1 << 1));} while (0)
#define GPIOC_REG_RST()					do { (RCC->IOPRSTR |= (1 << 2));	(RCC->IOPRSTR &= ~(1 << 2));} while (0)
#define GPIOD_REG_RST()					do { (RCC->IOPRSTR |= (1 << 3));	(RCC->IOPRSTR &= ~(1 << 3));} while (0)
#define GPIOF_REG_RST()					do { (RCC->IOPRSTR |= (1 << 5));	(RCC->IOPRSTR &= ~(1 << 5));} while (0)

/* Macro for EXTI */

#define GPIO_BASEADDR_TO_CODE(x)		((x == GPIOA) ? 0:\
										(x == GPIOB) ? 1 :\
										(x == GPIOC) ? 2 :\
										(x == GPIOD) ? 3 :\
										(x == GPIOF) ? 5 :0 )

/* Macros for IRQ Number from Vector table */

#define IRQ_NO_EXTI0_1					5
#define IRQ_NO_EXTI2_3					6
#define IRQ_NO_EXTI4_15					7


#define IRQ_SPI1						25
#define IRQ_SPI2						26

/* Macros for IRQ priority */

#define IRQ_PRIO_3						3
#define IRQ_PRIO_14						14


/* Generic Macros */

#define ENABLE								1
#define DISABLE								0
#define SET									ENABLE
#define RESET								DISABLE
#define GPIO_PIN_SET						SET
#define GPIO_PIN_RESET						RESET
#define FLAG_SET							SET
#define FLAG_RESET							RESET

/* SPI Register Bits */

/* SPI CR1 Register Bits */

#define SPIx_CR1_CPHA						0
#define SPIx_CR1_CPOL						1
#define SPIx_CR1_MSTR						2
#define SPIx_CR1_BR							3
#define SPIx_CR1_SPE						6
#define SPIx_CR1_LSBFIRST					7
#define SPIx_CR1_SSI						8
#define SPIx_CR1_SSM						9
#define SPIx_CR1_RXONLY						10
#define SPIx_CR1_CRCL						11
#define SPIx_CR1_CRCNEXT					12
#define SPIx_CR1_CRCEN						13
#define SPIx_CR1_BIDIOE						14
#define SPIx_CR1_BIDIMODE					15


/* SPI CR2 Register Bits */

#define SPIx_CR2_RXDMAEN						0
#define SPIx_CR2_TXDMAEN						1
#define SPIx_CR2_SSOE							2
#define SPIx_CR2_NSSP							3
#define SPIx_CR2_FRF							4
#define SPIx_CR2_ERRIE							5
#define SPIx_CR2_RXNEIE							6
#define SPIx_CR2_TXEIE							7
#define SPIx_CR2_DS								8
#define SPIx_CR2_FRXTH							12
#define SPIx_CR2_LDMA_RX						13
#define SPIx_CR2_LDMA_TX						14


/* SPI SR Register Bits */

#define SPIx_SR_RXNE						0
#define SPIx_SR_TXE							1
#define SPIx_SR_CHSIDE						2
#define SPIx_SR_UDR							3
#define SPIx_SR_CRCERR						4
#define SPIx_SR_MODF						5
#define SPIx_SR_OVR							6
#define SPIx_SR_BSY							7
#define SPIx_SR_FRE							8
#define SPIx_SR_FRLVL						9
#define SPIx_SR_FTLVL						11


/* GPIO Port Reset macros */
#define SPI1_REG_RST()					do { (RCC->APBRSTR2 |= (1 << 12));	(RCC->APBRSTR1 &= ~(1 << 12));} while (0) // setting and resetting the bit.
#define SPI2_REG_RST()					do { (RCC->APBRSTR1 |= (1 << 14));	(RCC->APBRSTR1 &= ~(1 << 14));} while (0)




#endif /* INC_STM32C031XX_H_ */
