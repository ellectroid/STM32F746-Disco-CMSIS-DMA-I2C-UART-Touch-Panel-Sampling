#include "nvic.h"

void nvic_setup(void) {

	NVIC_SetPriority(DMA2_Stream2_IRQn, 16U);
	NVIC_ClearPendingIRQ(DMA2_Stream2_IRQn);
	NVIC_EnableIRQ(DMA2_Stream2_IRQn);

	NVIC_SetPriority(DMA2_Stream7_IRQn, 16U);
	NVIC_ClearPendingIRQ(DMA2_Stream7_IRQn);
	NVIC_EnableIRQ(DMA2_Stream7_IRQn);

	NVIC_SetPriority(DMA1_Stream4_IRQn, 16U);
	NVIC_ClearPendingIRQ(DMA1_Stream4_IRQn);
	NVIC_EnableIRQ(DMA1_Stream4_IRQn);

	NVIC_SetPriority(DMA1_Stream2_IRQn, 16U);
	NVIC_ClearPendingIRQ(DMA1_Stream2_IRQn);
	NVIC_EnableIRQ(DMA1_Stream2_IRQn);

	NVIC_SetPriority(I2C3_EV_IRQn, 16U);
	NVIC_ClearPendingIRQ(I2C3_EV_IRQn);
	NVIC_EnableIRQ(I2C3_EV_IRQn);

	//timers enable interrupts in their counter enable functions
}

void DMA2_Stream2_IRQHandler(void) {
	if (DMA2->LISR & DMA_LISR_TCIF2) { //If stream 2 transfer complete (USART RX)
		DMA2->LIFCR |= DMA_LIFCR_CTCIF2; //clear flag
	}
}

void DMA2_Stream7_IRQHandler(void) {
	if (DMA2->HISR & DMA_HISR_TCIF7) { //If stream 7 transfer complete (USART TX)
		DMA2->HIFCR |= DMA_HIFCR_CTCIF7; //clear flag
	}
}

void DMA1_Stream4_IRQHandler(void) {
	if (DMA1->HISR & DMA_HISR_TCIF4) { //If stream 4 transfer complete (I2C3 TX)
		DMA1->HIFCR |= DMA_HIFCR_CTCIF4; //clear flag
	}
}

void DMA1_Stream2_IRQHandler(void) {
	if (DMA1->LISR & DMA_LISR_TCIF2) { //If stream 2 transfer complete (I2C3 RX)
		DMA1->LIFCR |= DMA_LIFCR_CTCIF2 | DMA_LIFCR_CTEIF2; //clear flags
		if (ReceivedTouchPointsStructure.receivedData[0] == 0xFF || ReceivedTouchPointsStructure.receivedData[0] == 0x00)
			return; //if number of touches detected is 0 (both 0x00 or 0xFF mean zero touches), stop
		ReceivedTouchPointsStructure.receivedData[0] += '0'; //convert unsigned int into a corresponding char (0 into '0', 1 into '1' etc.)
		usart_dma_sendArray(USART1, ReceivedTouchPointsStructure.receivedData, 0x01); //send number of touches over uart

	}
}

void TIM6_DAC_IRQHandler(void) {
	TIM6->SR = ~TIM_SR_UIF; //clear update event interrupt flag in timer peripheral
	i2c_dma_sendArray(I2C3, noOfTouchesRegister, sizeof(noOfTouchesRegister)); //write register name
	basic_timer_start(TIM7); //start one-pulse timer to trigger reception
}

void TIM7_IRQHandler(void) {
	TIM7->SR = ~TIM_SR_UIF; //clear update event interrupt flag in timer peripheral
	i2c_dma_receiveData(I2C3, ReceivedTouchPointsStructure.receivedData, 1); //receive number of touches
}

void I2C3_EV_IRQHandler(void) {
	I2C3->CR2 |= I2C_CR2_STOP; //the only I2C interrupt enabled is transfer complete. If that happens, send stop bit
}
