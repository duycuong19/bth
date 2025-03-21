#include "uart.h"

void My_USART_Init(uint32_t baud_rate) { // Ð?i tên hàm
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN;
    GPIOA->CRH &= ~(0xFF << 4);
    GPIOA->CRH |= (0x4B << 4); // PA9: AF_PP, PA10: Input floating
    USART1->BRR = 72000000 / baud_rate;
    USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

void USART_SendChar(char chr) {
    while (!(USART1->SR & USART_SR_TXE));
    USART1->DR = chr;
}

void USART_SendString(const char* str) {
    while (*str) USART_SendChar(*str++);
}

void USART_SendHex(uint8_t num) {
    char hex[] = "0123456789ABCDEF";
    USART_SendChar(hex[num >> 4]);
    USART_SendChar(hex[num & 0x0F]);
}
