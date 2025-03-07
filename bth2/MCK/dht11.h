#ifndef DHT11_H
#define DHT11_H

#include "stm32f10x.h"  // Thay d?i theo dòng STM32 c?a b?n

#define DHT11_PORT GPIOA
#define DHT11_PIN  GPIO_PIN_1

void DHT11_Init(void);
uint8_t DHT11_Read(uint8_t *temperature, uint8_t *humidity);

#endif