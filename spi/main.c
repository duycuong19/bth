#include "stm32f10x.h"       
#include "stm32f1xx_hal.h"  // Device header
#include "stm32f10x_spi.h"              // Keil::Device:StdPeriph Drivers:SPI
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "rfid.h"
#include "uart.h"
#include "gpio.h"
#include "timer.h"

// Hàm kh?i t?o h? th?ng
void Sys_Init(void) {
    Timer_Init();
    GPIO_InitLED();
    My_USART_Init(9600); // Ð?i tên hàm
    Delay_ms(200);
    RFID_Init();
    USART_SendString("RFID Reader Initialized\r\n");
    GPIO_Off();
}

// Hàm ch?y vòng l?p chính
void Sys_Run(void) {
    RFID_ReadCard();
    GPIO_ToggleLED();
}

int main(void) {
    Sys_Init();
    while (1) {
        Sys_Run();
    }
}