#include "dht11.h"
#include "stm32f10x_hal.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
void Delay_us(uint16_t us) {
    // H�m delay don gi?n b?ng v�ng l?p (chua t?i uu, c� th? d�ng TIM)
    volatile uint16_t count = us * 8; // STM32 ch?y 8MHz -> m?i v�ng l?p ~1us
    while (count--);
}

// C?u h�nh GPIO th�nh OUTPUT
void DHT11_SetPin_Output(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT11_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
}

// C?u h�nh GPIO th�nh INPUT
void DHT11_SetPin_Input(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT11_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
}

// Kh?i t?o giao ti?p v?i DHT11
void DHT11_Init(void) {
    DHT11_SetPin_Output();
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);
}

// G?i t�n hi?u START d?n DHT11
void DHT11_Start(void) {
    DHT11_SetPin_Output();
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_RESET);
    Delay_us(18000); // Gi? m?c th?p 18ms d? k�ch ho?t DHT11
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);
    Delay_us(30); // Gi? m?c cao trong 30us
    DHT11_SetPin_Input();
}

// �?c 1 bit t? DHT11
uint8_t DHT11_ReadBit(void) {
    while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_RESET); // Ch? m?c cao
    Delay_us(40); // Ch? 40us
    return (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_SET) ? 1 : 0;
}

// �?c 1 byte t? DHT11
uint8_t DHT11_ReadByte(void) {
    uint8_t value = 0;
    for (uint8_t i = 0; i < 8; i++) {
        value <<= 1;
        value |= DHT11_ReadBit();
        while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_SET); // Ch? m?c th?p
    }
    return value;
}

// �?c d? li?u t? DHT11
uint8_t DHT11_Read(uint8_t *temperature, uint8_t *humidity) {
    uint8_t rh_int, rh_dec, temp_int, temp_dec, checksum;

    DHT11_Start();

    // Ki?m tra ph?n h?i t? DHT11
    if (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_SET) return 1;
    while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_RESET);
    while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_SET);

    // �?c d? li?u
    rh_int = DHT11_ReadByte();
    rh_dec = DHT11_ReadByte();
    temp_int = DHT11_ReadByte();
    temp_dec = DHT11_ReadByte();
    checksum = DHT11_ReadByte();

    // Ki?m tra checksum
    if ((rh_int + rh_dec + temp_int + temp_dec) == checksum) {
        *humidity = rh_int;
        *temperature = temp_int;
        return 0; // OK
    }
    return 1; // L?i checksum
}