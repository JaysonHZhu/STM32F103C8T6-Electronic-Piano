#include "buzzer.h"
#include "../TIMER/timer.h"

void BUZZER_Init(void)
{
    TIM3_Buzzer_Init();
}

void BUZZER_SetFreq(uint16_t freq)
{
    TIM3_Buzzer_SetFreq(freq);
}

void BUZZER_Off(void)
{
    TIM3_Buzzer_Stop();
}
