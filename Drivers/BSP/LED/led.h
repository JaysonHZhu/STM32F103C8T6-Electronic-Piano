#ifndef __LED_H
#define __LED_H

#include "./SYSTEM/sys/sys.h"

#define LED0_PIN       GPIO_PIN_1
#define LED1_PIN       GPIO_PIN_2
#define LED2_PIN       GPIO_PIN_3
#define LED3_PIN       GPIO_PIN_4
#define LED4_PIN       GPIO_PIN_5
#define LED5_PIN       GPIO_PIN_6
#define LED6_PIN       GPIO_PIN_7
#define LED_ALL_PINS   (GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7)
#define LED_PORT       GPIOA

#define LED_0(x) HAL_GPIO_WritePin(LED_PORT, LED0_PIN, x ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define LED_1(x) HAL_GPIO_WritePin(LED_PORT, LED1_PIN, x ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define LED_2(x) HAL_GPIO_WritePin(LED_PORT, LED2_PIN, x ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define LED_3(x) HAL_GPIO_WritePin(LED_PORT, LED3_PIN, x ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define LED_4(x) HAL_GPIO_WritePin(LED_PORT, LED4_PIN, x ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define LED_5(x) HAL_GPIO_WritePin(LED_PORT, LED5_PIN, x ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define LED_6(x) HAL_GPIO_WritePin(LED_PORT, LED6_PIN, x ? GPIO_PIN_SET : GPIO_PIN_RESET)

#define LED0_Toggle() HAL_GPIO_TogglePin(LED_PORT, LED0_PIN)
#define LED1_Toggle() HAL_GPIO_TogglePin(LED_PORT, LED1_PIN)

void led_Init(void);
void LED_Set(uint8_t pattern);
void LED_AllOff(void);

#endif
