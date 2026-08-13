#include "timer.h"
#include "../BUZZER/buzzer.h"

static uint8_t timer_inited = 0;

void TIM3_Buzzer_Init(void)
{
    if (timer_inited) return;
    timer_inited = 1;
    __HAL_RCC_TIM3_CLK_ENABLE();
    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = BUZZER_PIN;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(BUZZER_PORT, &gpio);
    TIM3->PSC = 72 - 1;
    TIM3->CCMR2 = TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2;
    TIM3->CCER |= TIM_CCER_CC3E;
}

void TIM3_Buzzer_SetFreq(uint16_t freq)
{
    if (freq == 0) { TIM3_Buzzer_Stop(); return; }
    TIM3_Buzzer_Init();
    TIM3->ARR = 1000000 / freq - 1;
    TIM3->CCR3 = (1000000 / freq - 1) / 2;
    TIM3->EGR |= TIM_EGR_UG;
    TIM3->CR1 |= TIM_CR1_CEN;
}

void TIM3_Buzzer_Stop(void)
{
    TIM3->CR1 &= ~TIM_CR1_CEN;
}
