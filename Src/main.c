#include "main.h"

void system_hw_setup(void);

int main(void) {

	/*
	 * 	I2C and UART DMA basic CMSIS demo program for STM32F746-DISCO
	 *
	 * 	Hardware: STM32F746-DISCO with USART1 connected to ST-Link and I2C3 connected to FT5336 Capacitive Touch Panel (CTP)
	 *
	 * 	Peripherals used: USART1, I2C3, DMA1 (for I2C3), DMA2 (for USART1), TIM6 (Scanning CTP @ 60Hz), TIM7 (Scheduling CTP read command), SysTick, NVIC
	 *
	 * 	Operation:
	 *
	 * 	Set System Clock to 216MHz (max)
	 *
	 * 	USART1 DMA Test
	 * 	USART1 prints multiple messages via DMA, waits to receive 8 symbols, send them back
	 *
	 * 	I2C3 DMA Test
	 * 	I2C3 initializes FT5336
	 * 	TIM6 @ 60Hz and TIM7 Trigger Number of Touches Read. If number of touches detected is greater than 0, that number is sent via USART1 (DMA)
	 * 	Lasts indefinitely
	 *
	 * */




	system_hw_setup(); //initialize hardware

	/* --- UART DMA Test: Sending multiple lines, receiving line and sending it back --- */

	 uint8_t GreetingsLine1[] = "Testing DMA I2C & DMA UART on STM32F746-Disco!\r\n";
	 uint8_t GreetingsLine2[] = "USART1 DMA test. Enter 8 Symbols to be sent back to you (USART 1 connected to ST-Link): \r\n";
	 uint8_t NewLine[] = "\r\n";
	 uint8_t GreetingsLine3[] = "I2C test. The number of touches on FT5336 Capacitive Touch Panel on I2C3\r\nSampling @ 60Hz using timer; report via UART only if number of touches > 0; max = 5): \r\n";
	 uint8_t uart_dma_receiveBuffer[8];


	 usart_dma_sendArray(USART1, GreetingsLine1, sizeof(GreetingsLine1) - 1); //without end of line char
	 while (DMA2_Stream7->NDTR > 0x00); // wait for the first line to finish sending stuff over DMA (number of bytes to transfer reaches 0)

	 system_msdelay(2000); //wait 2s just because I want to show off systick

	 usart_dma_sendArray(USART1, GreetingsLine2, sizeof(GreetingsLine2) - 1); //without end of line char
	 usart_dma_receiveArray(USART1, uart_dma_receiveBuffer, 8); //wait for new input
	 while (DMA2_Stream2->NDTR > 0x00 || DMA2_Stream7->NDTR > 0x00); //wait while everything to be sent is sent and everything to be received is received

	 usart_dma_sendArray(USART1, uart_dma_receiveBuffer, sizeof(uart_dma_receiveBuffer)); //send what we received back
	 while (DMA2_Stream7->NDTR > 0x00); // wait for the first line to finish sending stuff over DMA (number of bytes to transfer reaches 0)

	 usart_dma_sendArray(USART1, NewLine, sizeof(NewLine) - 1); //force new line
	 while (DMA2_Stream7->NDTR > 0x00); // wait for data to be sent

	 usart_dma_sendArray(USART1, NewLine, sizeof(NewLine) - 1); //force new line
	 while (DMA2_Stream7->NDTR > 0x00); // wait for data to be sent

	 usart_dma_sendArray(USART1, GreetingsLine3, sizeof(GreetingsLine3) - 1); //force new line
	 while (DMA2_Stream7->NDTR > 0x00); // wait for data to be sent



	/* ---------- END OF UART DMA TEST ------------ */

	/* --- I2C3 DMA Test: report number of touches via UART --- */

	i2c_dma_sendArray(I2C3, interruptPollingMode, sizeof(interruptPollingMode)); //write interrupt register: polling mode

	basic_timer_start(TIM6); //start requesting data from touch panel @ 60Hz

	while (1) {
	}
}

void system_hw_setup(void) {
	rcc_setup(); //clock for peripheral, clock will not be altered; therefore default HSI 16MHz
	systick_setup(SYSTEM_FREQUENCY); //activate systick
	gpio_setup(); //set pin modes and functions
	dma_reset_flags(DMA2); //clear DMA2 flags for USART1
	dma_reset_flags(DMA1); //clear DMA1 flags for I2C3
	usart_dma_setup(USART1); //set control registers and settings for USART1 and its DMA connected to st-link
	usart_enable(USART1); //enable uart1
	usart_enable_tx(USART1); //enable tx line (wrapper)
	usart_enable_rx(USART1); //enable rx line (wrapper)
	i2c_dma_setup(I2C3); //set control registers and settings for I2C3 and its DMA connected to FT5336 capacitive touch panel
	basic_timer_setup(TIM6, 54000U, 2000U / 60U + 1U, 0); //TIM6 60Hz Update Event (Touch) to request data from touch panel
	basic_timer_setup(TIM7, 54U, 420U, 1U); //TIM7 210us one pulse mode (schedule I2C read 210us after I2C write)
	nvic_setup(); //set interrupts and their priorities
}
