/**
 ****************************************************************************************************
 * @file        usart.h
 * @author      JaysonHZhu
 * @version     V1.1
 * @date        2026-09-25
 * @brief       系统层：串口1（USART1）初始化与 printf 重定向（仅发送）
 * @license     MIT License, Copyright (c) 2026 JaysonHZhu
 ****************************************************************************************************
 */

#ifndef __USART_H
#define __USART_H

#include "stdio.h"
#include "./SYSTEM/sys/sys.h"

/* 串口1：PA9 = TX，PA10 = RX，本工程只用发送（printf 日志） */
#define USART_UX                USART1

#define USART_TX_GPIO_PORT      GPIOA
#define USART_TX_GPIO_PIN       GPIO_PIN_9
#define USART_RX_GPIO_PORT      GPIOA
#define USART_RX_GPIO_PIN       GPIO_PIN_10

extern UART_HandleTypeDef g_uart1_handle;   /* HAL UART 句柄 */

void usart_init(uint32_t baudrate);         /* 串口1初始化，例如 usart_init(115200) */

#endif
