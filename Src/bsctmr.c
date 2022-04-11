#include "bsctmr.h"

void basic_timer_setup(TIM_TypeDef *TMR, uint16_t PSC, uint16_t ARR, uint8_t OPM) {
	TMR->CR1 |= TIM_CR1_ARPE | TIM_CR1_URS; //ARR is buffered | only overflow/underflow generates update event.

	TMR->PSC = PSC - 1U; //set prescaler; 16-bit
	TMR->ARR = ARR - 1U; //set auto-reload register; 16-bit
	if (OPM == 0x00) //one pulse mode off
		TMR->CR1 &= ~TIM_CR1_OPM;
	else
		//one pulse mode on
		TMR->CR1 |= TIM_CR1_OPM;
	TMR->CNT = 0x00; //reset counter
}

void basic_timer_start(TIM_TypeDef *TMR) {
	TMR->DIER &= ~TIM_DIER_UIE; //update event interrupt disable
	if (TMR == TIM6) {
		NVIC_DisableIRQ(TIM6_DAC_IRQn);
	}
	if (TMR == TIM7) {
		NVIC_DisableIRQ(TIM7_IRQn);
	}

	TMR->EGR = TIM_EGR_UG; //force register update via force update event
	TMR->SR = ~TIM_SR_UIF; //force clear update event
	if (TMR == TIM6) {
		NVIC_SetPriority(TIM6_DAC_IRQn, 1U);
		NVIC_ClearPendingIRQ(TIM6_DAC_IRQn);
		NVIC_EnableIRQ(TIM6_DAC_IRQn);
	}
	if (TMR == TIM7) {
		NVIC_SetPriority(TIM7_IRQn, 1U);
		NVIC_ClearPendingIRQ(TIM7_IRQn);
		NVIC_EnableIRQ(TIM7_IRQn);
	}
	TMR->DIER |= TIM_DIER_UIE; //update event interrupt enable
	TMR->CR1 |= TIM_CR1_CEN; //enable timer counter

}

