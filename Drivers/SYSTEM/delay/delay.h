/**
 ****************************************************************************************************
 * @file        delay.h
 * @author      JaysonHZhu
 * @version     V1.0
 * @date        2026-09-24
 * @brief       系统层：基于 DWT 周期计数器的微秒/毫秒延时
 * @license     MIT License, Copyright (c) 2026 JaysonHZhu
 ****************************************************************************************************
 */

#ifndef __DELAY_H
#define __DELAY_H

#include "./SYSTEM/sys/sys.h"

void delay_init(uint16_t sysclk);   /* sysclk：系统主频(MHz)，仅当 SystemCoreClock 不可用时作兜底 */
void delay_us(uint32_t nus);        /* 微秒级延时 */
void delay_ms(uint16_t nms);        /* 毫秒级延时 */

#endif
