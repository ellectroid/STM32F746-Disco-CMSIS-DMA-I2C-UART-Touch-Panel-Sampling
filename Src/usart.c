#include <usart.h>

void usart_enable(USART_TypeDef *USART) {
	USART->CR1 |= USART_CR1_UE; //USART1 Enable
}
void usart_disable(USART_TypeDef *USART) {
	USART1->CR1 &= ~USART_CR1_UE; //USART1 Disable
}
void usart_enable_tx(USART_TypeDef *USART) {
	USART1->CR1 |= USART_CR1_TE; // USART Transmitter enabled
}
void usart_disable_tx(USART_TypeDef *USART) {
	USART1->CR1 &= ~USART_CR1_TE; //USART Transmitter disabled
}
void usart_enable_rx(USART_TypeDef *USART) {
	USART1->CR1 |= USART_CR1_RE; //USART Receiver enabled
}
void usart_disable_rx(USART_TypeDef *USART) {
	USART1->CR1 &= ~USART_CR1_RE; //USART Receiver disabled
}

void usart_dma_setup(USART_TypeDef *USART) {
	//Set all configs to Reset Value
	USART->CR1 = 0x00;
	USART->CR2 = 0x00;
	USART->CR3 = 0x00;
	USART->BRR = 0x00;
	USART->GTPR = 0x00;
	USART->RQR = 0x00;
	USART->ISR = 0x020000C0;

	USART->CR1 &= ~(USART_CR1_M); //Set word length 8;
	USART->CR3 |= USART_CR3_OVRDIS; //disable receiver overrun error
	USART->BRR = 0x683; //Set baudrate to 9600 (Fck = 16MHz)

	USART->CR3 |= (USART_CR3_DMAT | USART_CR3_DMAR);  //Enable DMA-mode receiver/transmitter

	usart_dma_rx_status_finished = 1U;
	usart_dma_tx_status_finished = 1U;

	//USART1 - DMA2 Stream 2 Channel 4 for RX, DMA2 Stream 7 Channel 4 for TX
}

void usart_dma_sendArray(USART_TypeDef *USART, uint8_t data[], uint32_t length) {
	if(!usart_dma_tx_status_finished) return;
	if (length < 1U)
		return; //idiot-proofing

	usart_dma_tx_status_finished = 0;

	DMA2_Stream7->CR = 0x00; //reset control register to default value; disable ongoing streams
	DMA2_Stream7->FCR = 0x21; //reset FIFO control register

	while (DMA2_Stream7->CR & DMA_SxCR_EN); //wait until stream is actually disabled

	DMA2->HIFCR |= (0x1 << 22U) | (0xF << 24); //Clear DMA2_Stream7 flags

	DMA2_Stream7->PAR = (uint32_t) (&(USART->TDR)); //set peripheral port address
	DMA2_Stream7->M0AR = (uint32_t) data; //set memory port address
	DMA2_Stream7->NDTR = length; //set number of data items to transfer
	DMA2_Stream7->CR |= 4U << 25U; //select channel 4

	DMA2_Stream7->CR &= ~DMA_SxCR_PL; //set stream priority level (lowest)

	DMA2_Stream7->CR |= DMA_SxCR_MINC; //set memory address increment by MSIZE

	DMA2_Stream7->CR |= DMA_SxCR_DIR_0; //set direction memory to peripheral

	DMA2_Stream7->CR |= DMA_SxCR_TCIE | DMA_SxCR_TEIE; //enable transfer complete interrupt and transfer error interrupt

	USART->ICR |= USART_ICR_TCCF; //clear transmission complete flag of USART

	DMA2_Stream7->CR |= DMA_SxCR_EN; //activate stream

}

void usart_dma_receiveArray(USART_TypeDef *USART, uint8_t data[], uint32_t length) {
	if(!usart_dma_rx_status_finished) return;
		if (length < 1U)
		return; //idiot-proofing

	usart_dma_rx_status_finished = 0;

	DMA2_Stream2->CR = 0x00; //reset DMA stream 2 control register
	DMA2_Stream2->FCR = 0x21; //reset FIFO control register
	while (DMA2_Stream2->CR & DMA_SxCR_EN); //wait until stream is actually disabled

	DMA2->LIFCR |= (1U << 16U) | (0x0F << 18U);  //clear DMA2_Stream2 flags

	DMA2_Stream2->PAR = (uint32_t) (&(USART->RDR)); //set peripheral port address
	DMA2_Stream2->M0AR = (uint32_t) data; //set memory port address
	DMA2_Stream2->NDTR = length; //set number of data items to transfer
	DMA2_Stream2->CR |= 4U << 25U; //select channel 4

	DMA2_Stream2->CR &= ~DMA_SxCR_PL; //set stream priority level (lowest)

	DMA2_Stream2->CR |= DMA_SxCR_MINC; //set memory address increment by MSIZE

	DMA2_Stream2->CR &= ~DMA_SxCR_DIR; //set direction peripheral to memory

	DMA2_Stream2->CR |= DMA_SxCR_TCIE | DMA_SxCR_TEIE; //enable transfer complete and transfer error interrupts

	DMA2_Stream2->CR |= DMA_SxCR_EN; //activate stream

}
