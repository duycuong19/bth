#ifndef __SPI_H__
#define __SPI_H__

#include "stm32f10x.h"
#include "stm32f10x_spi.h"

void SPI_Config(void);
uint8_t SPI_SendByte(uint8_t byte);

#endif
