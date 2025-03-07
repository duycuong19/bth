#include "dht11.h"

uint8_t temperature, humidity;

int main(void) {
    HAL_Init();
    __HAL_RCC_GPIOA_CLK_ENABLE(); // B?t clock GPIOA n?u c?n
    DHT11_Init();

    while (1) {
        if (DHT11_Read(&temperature, &humidity) == 0) {
            printf("Temperature: %d°C, Humidity: %d%%\r\n", temperature, humidity);
        } else {
            printf("DHT11 Read Error!\r\n");
        }
        HAL_Delay(2000); // Ð?c m?i 2 giây
    }
}