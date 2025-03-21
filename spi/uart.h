#ifndef __UART_H__
#define __UART_H__

#include "stm32f10x.h"

void My_USART_Init(uint32_t baud_rate); // Ð?i tên hàm
void USART_SendChar(char chr);
void USART_SendString(const char* str);
void USART_SendHex(uint8_t num);

#endif