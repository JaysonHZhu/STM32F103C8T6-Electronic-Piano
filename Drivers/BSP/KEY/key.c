#include "key.h"
#include "./SYSTEM/delay/delay.h"

void KEY_Init(void)
{
    GPIO_InitTypeDef gpio;
    __HAL_RCC_GPIOB_CLK_ENABLE();
    gpio.Pin = KEY_UP_PIN | KEY_DOWN_PIN | KEY_CONFIRM_PIN |
               KEY1_PIN | KEY2_PIN | KEY3_PIN | KEY4_PIN;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &gpio);
}

KeyValue KEY_Scan(void)
{
    static uint8_t last[7] = {1,1,1,1,1,1,1};
    static uint8_t hold[7] = {0,0,0,0,0,0,0};
    const uint16_t pins[7] = {KEY_UP_PIN, KEY_DOWN_PIN, KEY_CONFIRM_PIN,
                              KEY1_PIN, KEY2_PIN, KEY3_PIN, KEY4_PIN};
    const KeyValue map[7] = {KEY_UP, KEY_DOWN, KEY_CONFIRM,
                             KEY_1, KEY_2, KEY_3, KEY_4};

    uint8_t e1 = HAL_GPIO_ReadPin(KEY_PORT, KEY_EXIT1_PIN);
    uint8_t e2 = HAL_GPIO_ReadPin(KEY_PORT, KEY_EXIT2_PIN);
    if (e1 == 0 && e2 == 0) return KEY_EXIT;

    for (uint8_t i = 0; i < 7; i++) {
        uint8_t cur = HAL_GPIO_ReadPin(KEY_PORT, pins[i]);
        if (last[i] == 1 && cur == 0) hold[i] = 1;
        else if (cur == 1) hold[i] = 0;
        last[i] = cur;
        if (hold[i]) return map[i];
    }
    return KEY_NONE;
}

KeyValue KEY_Get(void)
{
    KeyValue k;
    while (1) {
        k = KEY_Scan();
        if (k != KEY_NONE) {
            delay_ms(20);
            while (KEY_Scan() != KEY_NONE);
            return k;
        }
    }
}

uint8_t KEY_IsExitPressed(void)
{
    return (KEY_Scan() == KEY_EXIT);
}
