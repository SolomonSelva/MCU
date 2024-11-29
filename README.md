# STM32 Bare-Metal Drivers and APIs

## Overview
This repository contains custom drivers and APIs for STM32 microcontrollers, developed using bare-metal programming techniques. These drivers directly interact with hardware registers to provide lightweight and efficient peripheral control, bypassing the need for heavyweight libraries like HAL or LL. 

The repository serves as a resource for embedded systems enthusiasts and professionals who want to explore low-level programming on ARM Cortex-M platforms.

---

## Features
- **GPIO Driver**: Configure pins as input/output, with options for pull-up, pull-down, and speed settings.
- **UART Driver**: Enable asynchronous communication with configurable baud rate, parity, and stop bits. (Will be implemented soon)
- **SPI Driver**: Support full-duplex and half-duplex communication modes with flexible clock configurations.
- **Timer APIs**: Generate PWM signals and implement precise delays using timers. (Will be implemented soon)
- **I2C Driver**: Facilitate communication with I2C-compatible devices, supporting master mode and various addressing schemes. (Will be implemented soon)
- Modular, reusable, and optimized for memory-constrained environments.

---

## Peripherals Supported
- **STM32 Family**: Tested on STM32F767ZI, but adaptable to other STM32 series.
- **Core Peripherals**: GPIO, UART, SPI, I2C, and TIM.
- Designed for **ARM Cortex-M processors**.

---

## Development Tools
- Microcontroller: STM32F767ZI
- Toolchain: ARM GCC, STM32CubeIDE
- Debugger: ST-Link V2
- Protocol Analyzer: Logic Analyzer for UART, SPI, and I2C debugging.
