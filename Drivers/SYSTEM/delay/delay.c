/**
 ****************************************************************************************************
 * @file        delay.c
 * @author      JaysonHZhu
 * @version     V1.0
 * @date        2026-09-24
 * @brief       系统层：基于 DWT 周期计数器(CYCCNT)的微秒/毫秒延时
 * @note        不使用 SysTick，因此不会影响 HAL 的 1 ms 节拍与 HAL_Delay()
 * @license     MIT License, Copyright (c) 2026 JaysonHZhu
 ****************************************************************************************************
 */

#include "./SYSTEM/delay/delay.h"

static uint32_t s_delay_clk_hz   = 0;   /* 初始化时记录的内核时钟频率(Hz) */
static uint32_t s_delay_us_ticks = 0;   /* 1 us 对应的内核时钟数 */
static uint8_t  s_delay_ready    = 0;   /* DWT 周期计数器是否已使能 */

/**
 * @brief       使能 DWT 周期计数器并计算 1 us 的节拍数
 * @param       无
 * @retval      无
 * @note        内部函数，可重复调用（幂等）
 */
static void delay_dwt_init(void)
{
    uint32_t clk = SystemCoreClock;

    if (clk == 0U) clk = s_delay_clk_hz;
    if (clk == 0U) clk = 72000000U;                     /* 兜底：F103 常规主频 */

    s_delay_us_ticks = clk / 1000000U;
    if (s_delay_us_ticks == 0U) s_delay_us_ticks = 1U;

    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;     /* 使能跟踪与调试模块 */
    DWT->CYCCNT = 0U;                                   /* 周期计数器清零 */
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;                /* 使能周期计数器 */

    s_delay_ready = 1U;
}

/**
 * @brief       延时初始化
 * @param       sysclk: 系统主频(MHz)，例如 72
 * @retval      无
 * @note        优先使用 SystemCoreClock（由 HAL_RCC_ClockConfig 更新），入参仅作兜底
 */
void delay_init(uint16_t sysclk)
{
    s_delay_clk_hz = (uint32_t)sysclk * 1000000U;
    delay_dwt_init();
}

/**
 * @brief       微秒级延时
 * @param       nus: 延时长度(us)，单次上限约 59 秒（32 位周期计数器量程限制）
 * @retval      无
 */
void delay_us(uint32_t nus)
{
    uint32_t start;
    uint32_t ticks;

    if (nus == 0U)
    {
        return;
    }

    if (s_delay_ready == 0U)
    {
        delay_dwt_init();                               /* 允许未显式初始化时兜底 */
    }

    ticks = nus * s_delay_us_ticks;
    start = DWT->CYCCNT;

    while ((DWT->CYCCNT - start) < ticks)               /* 无符号相减，自动处理计数器回绕 */
    {
        ;
    }
}

/**
 * @brief       毫秒级延时
 * @param       nms: 延时长度(ms)
 * @retval      无
 * @note        逐毫秒累加，避免 ticks 超过 32 位周期计数器量程
 */
void delay_ms(uint16_t nms)
{
    while (nms-- != 0U)
    {
        sys_iwdg_feed();                                /* 长延时期间保持看门狗，避免误复位 */
        delay_us(1000U);
    }
}
