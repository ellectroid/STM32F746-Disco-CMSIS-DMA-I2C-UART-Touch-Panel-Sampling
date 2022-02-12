#include "main.h"

int main(void) {
	rcc_setup(); //clock for peripheral, clock will not be altered; therefore default HSI 16MHz
	gpio_setup(); //set pin modes and functions
	dma_setup(DMA2); //clear DMA2 flags
	usart_dma_setup(USART1); //set control registers and settings for uart1
	usart_enable(USART1); //enable uart1
	usart_enable_tx(USART1); //enable tx line (wrapper)
	usart_enable_rx(USART1); //enable rx line (wrapper)
	nvic_setup(); //set interrupts and their priorities
	uint8_t myArray[] = "Sending and receiving stuff with DMA!\r\n";
	uint8_t myArray2[] = "Write into receive buffer of size 8. When it's full, will send it back\r\n";
	usart_dma_sendArray(USART1, myArray, sizeof(myArray)-1); //without end of line char

	while(!usart_dma_tx_status_finished); // the first line finish sending the greeting to load the second line

	usart_dma_sendArray(USART1, myArray2, sizeof(myArray2)-1); //without end of line char
	usart_dma_receiveArray(USART1, uart_dma_receiveBuffer, 8); //when buffer is received, it will be send back by DMA by triggering software interrupt. See nvic.c

	while(1)
	{
		for(uint32_t i = 0; i < 500000; i++); //lame pause
		blink_ld1();


	}
}
