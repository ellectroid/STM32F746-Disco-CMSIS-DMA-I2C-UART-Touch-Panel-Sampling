#ifndef NVIC_H_
#define NVIC_H_
#include "stm32f746xx.h"
#include "core_cm7.h"
#include "usart.h"

#define SoftwareIRQnHandler(void) CAN2_TX_IRQHandler(void)

enum redefinitions{
	SoftwareIRQn = CAN2_TX_IRQn
};

void nvic_setup(void);

#endif /* NVIC_H_ */
