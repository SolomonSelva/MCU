
#ifndef __CYCLE_PROFILING__
#define __CYCLE_PROFILING__
#include <stdint.h>
#include <stdio.h>
#include "stm32f767xx.h"
#include "stm32f767xx_dwt_driver.h"
#include "stm32f767xx_gpio_driver.h"

// Function to enable DWT_CYCCNT before main
__attribute__((constructor))
void initGPIO() {
    GPIO_LED_INIT();
}

#endif
        /*
 * 002_ExecutionTime_Test.c
 *
 *  Created on: Nov 17, 2024
 *      Author: root
 */

#include <stdio.h>
#include "stm32f767xx.h"
#include "stm32f767xx_dwt_driver.h"

int fastFunction() {
		/* Profiling code added for GPIO Toggling */
                GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);

	int sum = 0;
	for (int i = 0; i < 1000; i++) {
		sum += i;
	}
	printf("Sum in fastFunction: %d\n", sum);

	                GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
	            return 0;

}

void slowFunction() {
		/* Profiling code added for GPIO Toggling */
                GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);

	long long sum = 0;
	for (long long i = 0; i < 1000000; i++) {
		sum += i;
	}
	printf("Sum in slowFunction: %lld\n", sum);

                GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
            }

int main() {
		/* Profiling code added for GPIO Toggling */
                GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);

	printf("Starting program...\n");


	fastFunction();
	slowFunction();

	printf("Program finished.\n");

	                GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
	            return 0;
}
