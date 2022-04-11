#include "dma.h"

void dma_reset_flags(DMA_TypeDef *DMA){
	//force clear all flags
	DMA->LIFCR |= (1U<<0U) | (0x1F<<2U) | (0xF<<8U) | (1U<<16U) | (0x1F<<18U) | (0xF<<24);
	DMA->HIFCR |= (1U<<0U) | (0x1F<<2U) | (0xF<<8U) | (1U<<16U) | (0x1F<<18U) | (0xF<<24);
}
