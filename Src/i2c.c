#include "i2c.h"

void i2c_dma_setup(I2C_TypeDef *I2C) {

	//initialize variables
	noOfTouchesRegister[0] = FT5336_NO_OF_TOUCHES_REG; //load register address into array for DMA to send
	interruptPollingMode[0] = FT5336_INTERRUPT_MODE_REG; //load register address into array for DMA to send
	interruptPollingMode[1] = FT5336_INTERRUPT_MODE_REG_VALUE_POLLING; //load register value into array for DMA to send


	I2C->CR1 &= ~I2C_CR1_PE; //make sure the peripheral is off

	/*
	 * Analog filter enabled, digital noise filter disabled
	 * Interrupts disabled
	 * DMA TX enabled
	 * DMA RX enabled
	 * Transfer Complete Interrupt Enable
	 * */
	I2C->CR1 = 0x00; //reset
	I2C->CR1 |= (0x00 << I2C_CR1_ANFOFF_Pos) | (0x00 << I2C_CR1_DNF_Pos) | I2C_CR1_TXDMAEN | I2C_CR1_RXDMAEN | I2C_CR1_TCIE;

	/*
	 * Set WRITE mode
	 * 7-bit mode (ADD10=0)
	 * Load 7-bit Slave Address
	 * Autoend
	 */
	I2C->CR2 = 0x00; //reset
	I2C->CR2 |= (0x00 << I2C_CR2_RD_WRN_Pos) | (FT5336_I2C_SLAVE_ADDRESS << I2C_CR2_SADD_Pos);

	/*
	 * TIMING
	 * 100kHz I2C
	 * */
	I2C->TIMINGR = 0x00; //reset
	I2C->TIMINGR |= (0x06 << I2C_TIMINGR_PRESC_Pos) | (0x02 << I2C_TIMINGR_SCLDEL_Pos) | (0x00 << I2C_TIMINGR_SDADEL_Pos) | (0x1E << I2C_TIMINGR_SCLH_Pos)
			| (0x2A << I2C_TIMINGR_SCLL_Pos);




	//--------------- I2C3_TX DMA SETUP ----------------------

	DMA1_Stream4->CR = 0x00; //reset control register to default value; disable ongoing streams
	DMA1_Stream4->FCR = 0x21; //reset FIFO control register

	while (DMA1_Stream4->CR & DMA_SxCR_EN); //wait until stream is actually disabled

	DMA1->HIFCR |= (0x1 << 0U) | (0xF << 2U); //Clear DMA1_Stream4 flags

	DMA1_Stream4->PAR = (uint32_t) (&(I2C->TXDR)); //set peripheral port address

	/*
	 * DMA1 Stream 4 Channel 3
	 * Memory address increment by MSIZE
	 * Direction memory-to-peripheral
	 * Peripheral flow control
	 * Transfer complete interrupt
	 *
	 */

	DMA1_Stream4->CR |= 3U << 25U | DMA_SxCR_MINC | DMA_SxCR_DIR_0 | DMA_SxCR_PFCTRL | DMA_SxCR_TCIE;



	//------------------- I2C3_RX DMA SETUP -------------------------------

	DMA1_Stream2->CR = 0x00; //reset DMA stream 2 control register
	DMA1_Stream2->FCR = 0x21; //reset FIFO control register
	while (DMA1_Stream2->CR & DMA_SxCR_EN); //wait until stream is actually disabled

	DMA2->LIFCR |= (1U << 16U) | (0x0F << 18U);  //clear DMA2_Stream2 flags

	DMA1_Stream2->PAR = (uint32_t) (&(I2C->RXDR)); //set peripheral port address

	/*
	 * DMA1 Stream 2 Channel 3
	 * Memory increment by MSIZE
	 * Direction peripheral-to-memory (default)
	 * Peripheral flow control
	 * Transfer complete interrupt
	 * */
	DMA1_Stream2->CR |= 3U << 25U | DMA_SxCR_MINC | DMA_SxCR_PFCTRL | DMA_SxCR_TCIE;


	//------------- Activate peripheral --------------------

	I2C->CR1 |= I2C_CR1_PE; //activate peripheral

}

void i2c_dma_sendArray(I2C_TypeDef *I2C, uint8_t data[], uint8_t length) {
	DMA1_Stream4->CR &= ~DMA_SxCR_EN; //disable stream
	while(DMA1_Stream4->CR & DMA_SxCR_EN); //wait while stream disabled
	DMA1->HIFCR |= DMA_HIFCR_CTCIF4 | DMA_HIFCR_CTEIF4; //clear interrupt flags
	I2C->ICR |= I2C_ICR_STOPCF; //clear stop detection flag
	I2C->CR2 &= ~I2C_CR2_NBYTES; //clear number of bytes to send field
	I2C->CR2 |= (length << I2C_CR2_NBYTES_Pos); //set number of bytes to send
	I2C->CR2 &= ~I2C_CR2_RD_WRN; //master requests write transfer
	DMA1_Stream4->NDTR = length; //set number of data items to transfer
	DMA1_Stream4->M0AR = (uint32_t) data; //set memory port address
	DMA1_Stream4->CR |= DMA_SxCR_EN; //activate stream
	I2C->CR2 |= I2C_CR2_START; //set start bit to initiate transmission

}


void i2c_dma_receiveData(I2C_TypeDef *I2C, uint8_t data[], uint8_t length) {
	I2C->ICR |= I2C_ICR_STOPCF; //clear stop detection flag
	I2C->CR2 |= (1U << I2C_CR2_NBYTES_Pos); //set number of bytes to send (1 register)
	I2C->CR2 |= I2C_CR2_RD_WRN; //master requests read transfer
	DMA1_Stream2->NDTR = length; //set number of data items to transfer
	DMA1_Stream2->M0AR = (uint32_t) data; //set memory port address
	DMA1_Stream2->CR |= DMA_SxCR_EN; //activate stream
	I2C->CR2 |= I2C_CR2_START; //set start bit to initiate transmission

}
