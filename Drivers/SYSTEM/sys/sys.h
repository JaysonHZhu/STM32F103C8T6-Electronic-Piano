/**
 ****************************************************************************************************
 * @file        sys.h
 * @author      JaysonHZhu
 * @version     V1.0
 * @date        2026-09-24
 * @brief       系统层：系统时钟初始化（STM32F103C8T6 + ST HAL）
 * @license     MIT License, Copyright (c) 2026 JaysonHZhu
 ****************************************************************************************************
 */

#ifndef __SYS_H
#define __SYS_H

#include "stm32f1xx_hal.h"

/* 系统时钟初始化：HSE + PLL 倍频，HCLK = SYSCLK，PCLK1 = HCLK / 2，PCLK2 = HCLK */
void sys_stm32_clock_init(uint32_t plln);

/* 独立看门狗（IWDG）：约 2 s 溢出，启动后不可关闭 */
void sys_iwdg_init(void);
void sys_iwdg_feed(void);

#endif
