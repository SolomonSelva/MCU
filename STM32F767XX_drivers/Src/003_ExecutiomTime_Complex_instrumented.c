
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
 * 003_ExecutionTime_Test.c
 *
 *  Created on: Nov 19, 2024
 *      Author: root
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "stm32f767xx.h"
#include "stm32f767xx_dwt_driver.h"

// A function to compute factorial of a number (recursive)
long long factorial(int n) {
		/* Profiling code added for GPIO Toggling */
                GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);

    if (n <= 1) return 1;

                    GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
                return n * factorial(n - 1);
}

// A function to perform a bubble sort on an array
void bubbleSort(int arr[], int size) {
		/* Profiling code added for GPIO Toggling */
                GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);

    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // Swap arr[j] and arr[j + 1]
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

                GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
            }

// A function to simulate a time-consuming operation using trigonometric calculations
void trigOperations() {
		/* Profiling code added for GPIO Toggling */
                GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);

    double result = 0.0;
    for (int i = 0; i < 10000; i++) {
        result += sin(i) * cos(i);
    }
    printf("Result of trigOperations: %f\n", result);

                GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
            }

// A function to generate and print a large number of random numbers
void generateRandomNumbers(int count) {
		/* Profiling code added for GPIO Toggling */
                GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);

    for (int i = 0; i < count; i++) {
        printf("Random number %d: %d\n", i + 1, rand() % 100);
        if (i >= 9) break; // Limit printed output for demonstration
    }

                GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
            }

int main() {
		/* Profiling code added for GPIO Toggling */
                GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);

    printf("Starting program...\n");

    printf("Calling factorial function...\n");
    long long factResult = factorial(10);
    printf("Factorial of 10: %lld\n", factResult);

    printf("Calling bubbleSort function...\n");
    int arr[] = {5, 3, 8, 4, 2};
    bubbleSort(arr, sizeof(arr) / sizeof(arr[0]));
    printf("Sorted array: ");
    for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    printf("Calling trigOperations function...\n");
    trigOperations();

    printf("Calling generateRandomNumbers function...\n");
    generateRandomNumbers(1000);

    printf("Program finished.\n");

                    GPIO_WriteToOutputPin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
                return 0;
}
