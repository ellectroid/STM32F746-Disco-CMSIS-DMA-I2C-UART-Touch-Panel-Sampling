#include "rcc.h"

/*
 * Private functions
 */
void rcc_i2c3(void);
void rcc_gptimer12(void);
void rcc_gptimer5(void);
void rcc_gptimer3(void);
void rcc_usart1(void);
void rcc_bsctmr6(void);
void rcc_bsctmr7(void);
void rcc_advtmr1(void);
void rcc_dma1(void);
void rcc_dma2(void);
void rcc_gpio_porta(void);
void rcc_gpio_portb(void);
void rcc_gpio_porti(void);
void rcc_gpio_porth(void);
void rcc_init(void);

void rcc_setup(void) {
	rcc_init();
	rcc_usart1();
	rcc_gpio_porta();
	rcc_gpio_portb();
	rcc_gpio_porti();
	rcc_gpio_porth();
	rcc_dma2();
	rcc_i2c3();
	rcc_dma1();
	rcc_bsctmr6();
	rcc_bsctmr7();

}
void rcc_i2c3(void){
	RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
}

void rcc_gpio_porth(void){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
}

void rcc_gptimer12(void){
	RCC->APB1ENR |= RCC_APB1ENR_TIM12EN; //enable clock for general purpose timer 12
}
void rcc_gptimer5(void){
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN; //enable clock for general purpose timer 5
}
void rcc_gptimer3(void){
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; //enable clock for general purpose timer 3
}
void rcc_advtmr1(void){
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; //enable clock for advanced timer 1
}
void rcc_bsctmr6(void){
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN; //enable clock for basic timer 6
}
void rcc_bsctmr7(void){
	RCC->APB1ENR |= RCC_APB1ENR_TIM7EN; //enable clock for basic timer 7
}
void rcc_usart1(void) {
	RCC->DCKCFGR2 &= ~RCC_DCKCFGR2_USART1SEL; //reset USART1 clock source bits
	RCC->DCKCFGR2 |= RCC_DCKCFGR2_USART1SEL_0; //USART1 clock is system clock
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
void rcc_init(void) {

	//enable HSE
	RCC->CR |= RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY)); //wait while hardware signals HSE is stable

	//enable PWR peripheral
	volatile uint32_t temp;
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	temp = RCC->APB1ENR & RCC_APB1ENR_PWREN;
	(void) temp; //waste cycles until activated

	PWR->CR1 |= PWR_CR1_VOS; //explicit default value for internal voltage regulator
	(void) temp; //waste cycles until activated
	PWR->CR1 |= PWR_CR1_ODEN; //overdrive enable
	while (!(PWR->CSR1 & PWR_CSR1_ODRDY)); //wait while overdrive gets ready
	PWR->CR1 |= PWR_CR1_ODSWEN; //enable overdrive switching
	while (!(PWR->CSR1 & PWR_CSR1_ODSWRDY)); //wait while overdrive switching gets ready

	//configure PLL

	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE; //PLL/PLL2S source = HSE
	RCC->PLLCFGR &= ~(0x1F << RCC_PLLCFGR_PLLM_Pos); //reset all PLLM bits
	RCC->PLLCFGR |= (0x19 << RCC_PLLCFGR_PLLM_Pos); //PLLM 25
	RCC->PLLCFGR &= ~(0x1FF << RCC_PLLCFGR_PLLN_Pos); //reset all PLLN bits
	RCC->PLLCFGR |= (0x1B0 << RCC_PLLCFGR_PLLN_Pos); //PLLN 432
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP; //PLLP 2

	//RCC->PLLCFGR = 0x29406C19; //same value hardcoded

	RCC->CR |= RCC_CR_PLLON; //Enable PLL
	while (!(RCC->CR & RCC_CR_PLLRDY)); //wait while hardware signals PLL is OK

	FLASH->ACR |= 7U; //Set Flash wait states higher before pumping the clock up (8 clock cycles)
	// otherwise the flash can't serve instructions quickly enough for that CPU clock

	//configure AHB/APB clocks
	RCC->CFGR &= ~RCC_CFGR_HPRE_DIV1; //AHB Prescaler 1
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4; //APB1 Prescaler 4
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2; //APB2 Prescaler 2

	//Set System Clock to PLL
	RCC->CFGR |= RCC_CFGR_SW_PLL; // System Clock Mux: Select PLL as system clock
	while ((RCC->CFGR & RCC_CFGR_SWS) != (RCC_CFGR_SWS_PLL)); //wait until PLL is established as system clock
}

