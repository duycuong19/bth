#include "gpio.h"
#include "timer.h"

void GPIO_InitLED(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = LED_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);
}

void GPIO_ToggleLED(void) {
    LED_PORT->ODR ^= LED_PIN;
    Delay_ms(500);
}

void GPIO_Off(void) {
    LED_PORT->ODR &= ~(unsigned)LED_PIN;
}

void GPIO_On(void) {
    LED_PORT->ODR |= LED_PIN;
}
