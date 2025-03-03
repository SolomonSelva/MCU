/*
 * stm32f767xx_rcc_driver.h
 *
 *  Created on: Feb 3, 2025
 *      Author: solom
 */

#ifndef INC_STM32F767XX_RCC_DRIVER_H_
#define INC_STM32F767XX_RCC_DRIVER_H_

//This returns the APB1 clock value
uint32_t RCC_GetPCLK1Value(void);

//This returns the APB2 clock value
uint32_t RCC_GetPCLK2Value(void);

uint32_t  RCC_GetPLLOutputClock();

#endif /* INC_STM32F767XX_RCC_DRIVER_H_ */
