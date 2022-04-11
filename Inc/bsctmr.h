#ifndef BSCTMR_H_
#define BSCTMR_H_
#include "stm32f746xx.h"
#include "core_cm7.h"
#include "gpio.h"


void basic_timer_setup(TIM_TypeDef *TMR, uint16_t PSC, uint16_t ARR, uint8_t OPM);
void basic_timer_start(TIM_TypeDef *TMR);


#endif /* BSCTMR_H_ */
