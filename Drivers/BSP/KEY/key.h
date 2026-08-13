#ifndef __KEY_H
#define __KEY_H

#include "stm32f1xx_hal.h"

#define KEY_UP_PIN      GPIO_PIN_1
#define KEY_DOWN_PIN    GPIO_PIN_10
#define KEY_CONFIRM_PIN GPIO_PIN_11
#define KEY_PORT        GPIOB

#define KEY1_PIN        GPIO_PIN_12
#define KEY2_PIN        GPIO_PIN_13
#define KEY3_PIN        GPIO_PIN_14
#define KEY4_PIN        GPIO_PIN_15

#define KEY_EXIT1_PIN   KEY3_PIN
#define KEY_EXIT2_PIN   KEY4_PIN

typedef enum {
    KEY_NONE = 0,
    KEY_UP,
    KEY_DOWN,
    KEY_CONFIRM,
    KEY_EXIT,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4
} KeyValue;

void KEY_Init(void);
KeyValue KEY_Scan(void);
KeyValue KEY_Get(void);
uint8_t KEY_IsExitPressed(void);

#endif
