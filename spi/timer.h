#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f10x.h"
#include "stm32f10x_tim.h"

void Timer_Init(void);
void Delay_ms(uint32_t ms);

#endif