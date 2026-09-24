#ifndef __BUZZER_H
#define __BUZZER_H

#include "stm32f1xx_hal.h"

#define BUZZER_PORT       GPIOB
#define BUZZER_PIN        GPIO_PIN_0

void BUZZER_Init(void);
void BUZZER_SetFreq(uint16_t freq);
void BUZZER_Off(void);

#endif
