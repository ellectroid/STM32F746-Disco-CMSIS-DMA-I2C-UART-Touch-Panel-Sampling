#include "systick.h"

volatile uint32_t SYSTEM_MS; //46 days

void systick_setup(uint32_t sys_freq) {
	SysTick->LOAD = (SYSTEM_FREQUENCY / 1000U) - 1U; //1ms tick
	SysTick->VAL = 0x00; //explicitly set start value (undefined on reset)
	SysTick->CTRL |= (1U << SysTick_CTRL_TICKINT_Pos) | (1U << SysTick_CTRL_CLKSOURCE_Pos); //enable systick interrupt, source processor clock
	SCB->SHPR[8] = 0U; //set SysTick interrupt priority (default: 0, the highest)
	SysTick->CTRL |= (1U << SysTick_CTRL_ENABLE_Pos); //enable SysTick
}

void SysTick_Handler(void) {
	SYSTEM_MS++;
}

uint32_t millis(void){
	return SYSTEM_MS;
}

void system_msdelay(uint32_t delay_ms){
	uint32_t end = millis() + delay_ms;  //will overflow eventually
	while(millis()!=end); //not < in case of overflow
}
