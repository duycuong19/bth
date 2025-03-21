#ifndef __GPIO_H__
#define __GPIO_H__

#include "stm32f10x.h"

#define LED_PIN  GPIO_Pin_13
#define LED_PORT GPIOC

void GPIO_InitLED(void);
void GPIO_ToggleLED(void);
void GPIO_Off(void);
void GPIO_On(void);

#endif