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
	USART->GTPR = 0x00;

	USART->CR3 |= USART_CR3_OVRDIS; //disable receiver overrun error
	USART->BRR = 0x57E4; //Set baudrate to 9600 (Fck = 216MHz)

	USART->CR3 |= (USART_CR3_DMAT | USART_CR3_DMAR);  //Enable DMA-mode receiver/transmitter



	//------------------------- USART_TX DMA SETUP ----------------------

	DMA2_Stream7->CR = 0x00; //reset control register to default value; disable ongoing streams
	DMA2_Stream7->FCR = 0x21; //reset FIFO control register

	while (DMA2_Stream7->CR & DMA_SxCR_EN); //wait until stream is actually disabled

	DMA2->HIFCR |= (0x1 << 22U) | (0xF << 24); //Clear DMA2_Stream7 flags

	DMA2_Stream7->PAR = (uint32_t) (&(USART->TDR)); //set peripheral port address

	/*
	 *  Select channel 4
	 *  Memory address increment by MSIZE
	 *  Direction memory-to-peripheral
	 *  DMA flow control (default)
	 *  Transfer complete interrupt
	 *
	 * */
	DMA2_Stream7->CR |= 4U << 25U | DMA_SxCR_MINC | DMA_SxCR_DIR_0 | DMA_SxCR_TCIE;




	//----------------------- USART RX DMA SETUP -------------------------------

	DMA2_Stream2->CR = 0x00; //reset DMA stream 2 control register
	DMA2_Stream2->FCR = 0x21; //reset FIFO control register
	while (DMA2_Stream2->CR & DMA_SxCR_EN); //wait until stream is actually disabled

	DMA2->LIFCR |= (1U << 16U) | (0x0F << 18U);  //clear DMA2_Stream2 flags

	DMA2_Stream2->PAR = (uint32_t) (&(USART->RDR)); //set peripheral port address

	/*
	 *  Select channel 4
	 *  Memory address increment by MSIZE
	 *  Direction peripheral-to-memory (default)
	 *  DMA flow control (default)
	 *  Transfer complete interrupt
	 * */

	DMA2_Stream2->CR |= 4U << 25U | DMA_SxCR_MINC | DMA_SxCR_TCIE;

}

void usart_dma_sendArray(USART_TypeDef *USART, uint8_t data[], uint32_t length) {
	if (length < 1U)
		return; //idiot-proofing

	DMA2_Stream7->M0AR = (uint32_t) data; //set memory port address
	DMA2_Stream7->NDTR = length; //set number of data items to transfer

	USART->ICR |= USART_ICR_TCCF; //clear transmission complete flag of USART

	DMA2_Stream7->CR |= DMA_SxCR_EN; //activate stream

}

void usart_dma_receiveArray(USART_TypeDef *USART, uint8_t data[], uint32_t length) {
	if (length < 1U)
		return; //idiot-proofing

	DMA2_Stream2->M0AR = (uint32_t) data; //set memory port address
	DMA2_Stream2->NDTR = length; //set number of data items to transfer

	DMA2_Stream2->CR |= DMA_SxCR_EN; //activate stream

}
