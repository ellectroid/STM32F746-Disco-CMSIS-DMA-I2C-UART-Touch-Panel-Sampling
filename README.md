# STM32F746-DISCO-DMA-I2C-DMA-UART-TOUCH-PANEL-SAMPLING
Minimal I2C/UART DMA demo for STM32F746-DISCO

UART1 ST-Link COM Port

Clock: 216MHz.
USART1 is the USART connected to ST-Link that makes virtual COM port on PC.
I2C3 is connected to FT5336 capacitive touch panel.
TIM6 and TIM7 provide regular touch panel scanning.

This project is a proof of concept (that I can).

Might be useful for someone who wants to figure out how the stuff works.
I commented almost every line to make it simple to understand.


Hardware:
STM32F746 Disco board, Windows PC


Software:
STM32CubeIDE, Putty for UART communication;
No libraries used except for CMSIS register definitions.