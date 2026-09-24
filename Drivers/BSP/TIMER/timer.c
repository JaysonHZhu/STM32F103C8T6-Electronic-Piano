#include "timer.h"
#include "../BUZZER/buzzer.h"

static uint8_t  timer_inited = 0;
static uint32_t tim3_tick_hz = 1000000U;    /* TIM3 计数频率(Hz) */

/* 依据 APB1 时钟设置预分频，使 TIM3 计数频率为 1 MHz（不再硬编码 72 MHz） */
static void TIM3_Buzzer_ClockInit(void)
{
    RCC_ClkInitTypeDef clk_cfg;
    uint32_t latency;
    uint32_t pclk1;
    uint32_t tim_clk;

    HAL_RCC_GetClockConfig(&clk_cfg, &latency);
    pclk1 = HAL_RCC_GetPCLK1Freq();
    /* APB1 预分频不为 1 时，定时器时钟为 PCLK1 的两倍 */
    tim_clk = (clk_cfg.APB1CLKDivider == RCC_HCLK_DIV1) ? pclk1 : (pclk1 * 2U);
    TIM3->PSC = (uint16_t)(tim_clk / tim3_tick_hz - 1U);
}

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
    TIM3_Buzzer_ClockInit();
    TIM3->CCMR2 = TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2;
    TIM3->CCER |= TIM_CCER_CC3E;
}

void TIM3_Buzzer_SetFreq(uint16_t freq)
{
    if (freq == 0) { TIM3_Buzzer_Stop(); return; }
    TIM3_Buzzer_Init();
    TIM3->ARR = tim3_tick_hz / freq - 1U;
    TIM3->CCR3 = (tim3_tick_hz / freq - 1U) / 2U;
    TIM3->EGR |= TIM_EGR_UG;
    TIM3->CR1 |= TIM_CR1_CEN;
}

void TIM3_Buzzer_Stop(void)
{
    TIM3->CR1 &= ~TIM_CR1_CEN;
}
