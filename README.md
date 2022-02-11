# STM32F746-Minimal-UART-DMA-Driver-Demo
Minimal DMA-based UART driver for STM32F746

Minimalist DMA-based UART driver demo for STM32F746-Disco UART1 ST-Link COM Port

Clock: default, no changes (16MHz HSI).
USART1 is the USART connected to ST-Link that makes virtual COM port on PC.


This project is a proof of concept (that I can).

Might be useful for someone who wants to figure out how the stuff works.
I commented almost every line to make it simple to understand.


Hardware:
STM32F746 Disco board, Windows PC


Software:
STM32CubeIDE, Putty for UART communication;
No libraries used except for CMSIS register definitions.