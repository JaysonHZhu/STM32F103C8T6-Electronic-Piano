/**
 ****************************************************************************************************
 * @file        sys.c
 * @author      JaysonHZhu
 * @version     V1.0
 * @date        2026-09-24
 * @brief       系统层：系统时钟初始化（STM32F103C8T6 + ST HAL）
 * @license     MIT License, Copyright (c) 2026 JaysonHZhu
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"

/**
 * @brief       系统时钟初始化：HSE 起振 -> PLL 倍频 -> 切换 SYSCLK 到 PLL
 * @param       plln: PLL 倍频系数，例如 RCC_PLL_MUL9（8 MHz x 9 = 72 MHz）
 * @note        时钟树：HCLK = SYSCLK，PCLK1 = HCLK / 2，PCLK2 = HCLK，Flash 等待周期 2WS
 * @retval      无
 */
void sys_stm32_clock_init(uint32_t plln)
{
    RCC_OscInitTypeDef rcc_osc_init = {0};
    RCC_ClkInitTypeDef rcc_clk_init = {0};

    rcc_osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;   /* 使用外部晶振 */
    rcc_osc_init.HSEState       = RCC_HSE_ON;               /* 打开 HSE */
    rcc_osc_init.HSEPredivValue = RCC_HSE_PREDIV_DIV1;      /* HSE 不分频 */
    rcc_osc_init.PLL.PLLState   = RCC_PLL_ON;               /* 打开 PLL */
    rcc_osc_init.PLL.PLLSource  = RCC_PLLSOURCE_HSE;        /* PLL 时钟源为 HSE */
    rcc_osc_init.PLL.PLLMUL     = plln;                     /* PLL 倍频系数 */

    if (HAL_RCC_OscConfig(&rcc_osc_init) != HAL_OK)
    {
        while (1);                                          /* HSE / PLL 起振失败：停在此处便于定位 */
    }

    rcc_clk_init.ClockType      = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    rcc_clk_init.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;  /* 系统时钟来自 PLL */
    rcc_clk_init.AHBCLKDivider  = RCC_SYSCLK_DIV1;          /* AHB = SYSCLK */
    rcc_clk_init.APB1CLKDivider = RCC_HCLK_DIV2;            /* APB1 = HCLK / 2 */
    rcc_clk_init.APB2CLKDivider = RCC_HCLK_DIV1;            /* APB2 = HCLK */

    if (HAL_RCC_ClockConfig(&rcc_clk_init, FLASH_LATENCY_2) != HAL_OK)
    {
        while (1);                                          /* 时钟切换失败：停在此处便于定位 */
    }
}

/**
 * @brief       启动独立看门狗（IWDG）
 * @note        LSI 约 40 kHz，预分频 64 -> 计数时钟 625 Hz，重装载 1250 -> 溢出约 2 s。
 *              直接操作寄存器，因此不依赖 HAL 的 IWDG 模块（工程未包含 hal_iwdg.c）。
 *              调试冻结位 DBG_IWDG_STOP 由 Keil 的 .dbgconf 提供，
 *              断点暂停内核时计数器会一起停住，不会因调试触发复位。
 * @retval      无
 */
void sys_iwdg_init(void)
{
    IWDG->KR  = 0x5555U;        /* 允许写 PR / RLR */
    IWDG->PR  = 4U;             /* 预分频 64（4 * 2^4 = 64） */
    IWDG->RLR = 1250U;          /* 溢出时间约 2 s */
    IWDG->KR  = 0xAAAAU;        /* 装载计数器 */
    IWDG->KR  = 0xCCCCU;        /* 启动看门狗（启动后只能复位才能关闭） */
}

/**
 * @brief       喂狗
 * @param       无
 * @retval      无
 * @note        长时间延时（delay_ms）、按键等待与主循环中都会调用
 */
void sys_iwdg_feed(void)
{
    IWDG->KR = 0xAAAAU;
}
