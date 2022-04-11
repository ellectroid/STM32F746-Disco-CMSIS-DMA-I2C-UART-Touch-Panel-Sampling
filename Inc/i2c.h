#ifndef I2C_H_
#define I2C_H_

#include "stm32f746xx.h"

#define FT5336_I2C_SLAVE_ADDRESS 					((uint8_t)0x70)

#define FT5336_NO_OF_TOUCHES_REG 					((uint8_t)0x02)

#define FT5336_INTERRUPT_MODE_REG 					((uint8_t)0xA4)

#define FT5336_INTERRUPT_MODE_REG_VALUE_POLLING 	((uint8_t)0x00)

//this struct can include coordinates and other data for further development
struct TouchPoints{
	uint8_t receivedData[1];
} ReceivedTouchPointsStructure;


volatile uint8_t noOfTouches;  //MAX = 5
uint8_t noOfTouchesRegister[1];
uint8_t interruptPollingMode[2];

void i2c_dma_setup(I2C_TypeDef *I2C);
void i2c_dma_sendArray(I2C_TypeDef *I2C, uint8_t data[], uint8_t length);
void i2c_dma_receiveData(I2C_TypeDef *I2C, uint8_t data[], uint8_t length);

#endif /* I2C_H_ */
