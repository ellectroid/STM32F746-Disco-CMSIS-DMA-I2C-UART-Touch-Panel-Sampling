#ifndef USART_H_
#define USART_H_
#include "stm32f746xx.h"

void usart_dma_setup(USART_TypeDef *USART);
void usart_dma_sendArray(USART_TypeDef *USART, uint8_t data[], uint32_t length);
void usart_dma_receiveArray(USART_TypeDef *USART, uint8_t data[], uint32_t length);

void usart_enable(USART_TypeDef *USART);
void usart_disable(USART_TypeDef *USART);
void usart_enable_tx(USART_TypeDef *USART);
void usart_disable_tx(USART_TypeDef *USART);
void usart_enable_rx(USART_TypeDef *USART);
void usart_disable_rx(USART_TypeDef *USART);

uint8_t usart_dma_rx_status_finished;
uint8_t usart_dma_tx_status_finished;

uint8_t uart_dma_receiveBuffer[8];

#endif /* USART_H_ */
