/*
 * stm32f767xx_dwt_driver.h
 *
 *  Created on: Nov 16, 2024
 *      Author: root
 */

#ifndef INC_STM32F767XX_DWT_DRIVER_H_
#define INC_STM32F767XX_DWT_DRIVER_H_

#define DEMCR_TRACENA											24

#define DWT_CYCCNTENA											0

void DWT_CYCCNT_EN();

void GPIO_LED_INIT(void);

#endif /* INC_STM32F767XX_DWT_DRIVER_H_ */
