#include "rcc.h"

/*
 * Private functions
 */
void rcc_usart1(void);
void rcc_dma1(void);
void rcc_dma2(void);
void rcc_gpio_porta(void);
void rcc_gpio_portb(void);
void rcc_gpio_porti(void);

void rcc_setup(void) {
	rcc_usart1();
	rcc_gpio_porta();
	rcc_gpio_portb();
	rcc_gpio_porti();
	rcc_dma2();
}

void rcc_usart1(void) {
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; //enable clock for USART1 peripheral
}

void rcc_gpio_porta(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; //enable clock for GPIO port A
}

void rcc_gpio_portb(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; //enable clock for GPIO port B
}

void rcc_gpio_porti(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN; //enable clock for GPIO port I
}
void rcc_dma1(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN; //enable clock for DMA1
}
void rcc_dma2(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN; //enable clock for DMA2
}
