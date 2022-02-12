#include "nvic.h"

void nvic_setup(void) {

	NVIC_SetPriority(DMA2_Stream2_IRQn, 16U);
	NVIC_EnableIRQ(DMA2_Stream2_IRQn);

	NVIC_SetPriority(DMA2_Stream7_IRQn, 16U);
	NVIC_EnableIRQ(DMA2_Stream7_IRQn);

	NVIC_SetPriority(SoftwareIRQn, 16U);  //CAN2_TX Interrupt is used for software interrupt only!!!
	NVIC_EnableIRQ(SoftwareIRQn);

}

void DMA2_Stream2_IRQHandler(void) {
	NVIC_ClearPendingIRQ(DMA2_Stream2_IRQn);
	if (DMA2->LISR & DMA_LISR_TCIF2) { //If stream 2 transfer complete (USART RX)
		DMA2->LIFCR |= DMA_LIFCR_CTCIF2;
		usart_dma_rx_status_finished = 1;
		NVIC->STIR = CAN2_TX_IRQn; //trigger software interrupt for sending out the received data
	}
}

void DMA2_Stream7_IRQHandler(void) {
	NVIC_ClearPendingIRQ(DMA2_Stream7_IRQn);
	if (DMA2->HISR & DMA_HISR_TCIF7) { //If stream 7 transfer complete (USART TX)
		DMA2->HIFCR |= DMA_HIFCR_CTCIF7;
		usart_dma_tx_status_finished = 1;
	}
}

void SoftwareIRQnHandler(void){
	NVIC_ClearPendingIRQ(SoftwareIRQn);
	usart_dma_sendArray(USART1, uart_dma_receiveBuffer, 8); //send received data back
}
