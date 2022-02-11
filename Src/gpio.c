#include "gpio.h"

/*
 * Private functions
 */

void gpio_setup_porta(void);
void gpio_setup_portb(void);
void gpio_setup_porti(void);

void gpio_setup(void) {
	gpio_setup_porta();
	gpio_setup_portb();
	gpio_setup_porti();
}

void gpio_setup_porta(void) {
	/*
	 * PA9 USART1 TX
	 */
	GPIOA->MODER = (GPIOA->MODER & ~(GPIO_MODER_MODER9_0)) | GPIO_MODER_MODER9_1; //PA9 set to alternate function
	GPIOA->AFR[1] = (GPIOA->AFR[1] & ~(1U << 7)) | (0x07 << 4); //PA9 to AF7
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT9; //PA9 output push-pull
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR9; //PA9 output speed very high
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR9; //PA9 no pull-up no pull-down

}
void gpio_setup_portb(void) {
	/*
	 * PB7 USART1 RX
	 */
	GPIOB->MODER = (GPIOB->MODER & ~(GPIO_MODER_MODER7_0)) | (GPIO_MODER_MODER7_1); //PB7 set to alternate function
	GPIOB->AFR[0] = (GPIOB->AFR[0] & ~(1U << 31)) | (0x07 << 28); //PB7 to AF7
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT_7); //PB7 output push-pull
	GPIOB->OSPEEDR |= (GPIO_OSPEEDR_OSPEEDR7); //PB7 output speed very high
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR7); //PB7 no pull-up no pull-down

}
void gpio_setup_porti(void) {
	/*
	 *  PI1 GREEN LED LD1 ACTIVE HIGH
	 * */
	GPIOI->MODER = (GPIOI->MODER & ~(1U << 3U)) | (1U << 2U); //PI1 set to general purpose output mode
	GPIOI->OTYPER &= ~(GPIO_OTYPER_OT_1); //PI1 output push-pull
	GPIOI->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEEDR1); //PI1 output speed very low
	GPIOI->PUPDR &= ~(GPIO_PUPDR_PUPDR1); //PI1 no pull-up no pull-down
}

void blink_ld1(void) {
	GPIOI->ODR |= GPIO_ODR_OD1; //set PI1 to high
	for (uint32_t i = 0; i < 10000U; i++); //lame delay
	GPIOI->ODR &= ~GPIO_ODR_OD1; //set PI1 to low
}
