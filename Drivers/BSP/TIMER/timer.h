#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f1xx_hal.h"

void TIM3_Buzzer_Init(void);
void TIM3_Buzzer_SetFreq(uint16_t freq);
void TIM3_Buzzer_Stop(void);

#endif
