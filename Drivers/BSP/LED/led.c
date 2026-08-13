#include "./BSP/LED/led.h"

void led_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = LED_ALL_PINS;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &gpio);
    LED_AllOff();
}

void LED_Set(uint8_t pattern)
{
    uint16_t val = ((uint16_t)pattern << 1) & LED_ALL_PINS;
    for (uint8_t i = 1; i <= 7; i++) {
        HAL_GPIO_WritePin(LED_PORT, 1 << i, (val >> i) & 1 ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}

void LED_AllOff(void)
{
    HAL_GPIO_WritePin(LED_PORT, LED_ALL_PINS, GPIO_PIN_RESET);
}
