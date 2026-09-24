/**
 ****************************************************************************************************
 * @file        usart.c
 * @author      JaysonHZhu
 * @version     V1.1
 * @date        2026-09-25
 * @brief       系统层：串口1（USART1）初始化与 printf 重定向（仅发送）
 * @license     MIT License, Copyright (c) 2026 JaysonHZhu
 ****************************************************************************************************
 */

#include "./SYSTEM/usart/usart.h"

UART_HandleTypeDef g_uart1_handle;              /* UART 句柄 */

/******************************************************************************************/
/* printf 重定向到串口1：不使用半主机模式，也不依赖 MicroLIB */

#if (__ARMCC_VERSION >= 6010050)                /* AC6 编译器 */
__asm(".global __use_no_semihosting\n\t");
__asm(".global __ARM_use_no_argv \n\t");
#else                                           /* AC5 编译器 */
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
};

#endif

int _ttywrch(int ch)
{
    ch = ch;
    return ch;
}

void _sys_exit(int x)
{
    x = x;
}

char *_sys_command_string(char *cmd, int len)
{
    (void)cmd;
    (void)len;
    return NULL;
}

FILE __stdout;

int fputc(int ch, FILE *f)
{
    (void)f;

    while ((USART_UX->SR & USART_SR_TXE) == 0U)  /* 等待发送数据寄存器空 */
    {
        ;
    }

    USART_UX->DR = (uint8_t)ch;
    return ch;
}
/******************************************************************************************/

/**
 * @brief       串口1初始化（8 位数据、1 位停止位、无校验、无硬件流控）
 * @param       baudrate: 波特率，例如 115200
 * @retval      无
 * @note        本工程仅使用发送（printf 日志），未启用接收中断
 */
void usart_init(uint32_t baudrate)
{
    g_uart1_handle.Instance          = USART_UX;
    g_uart1_handle.Init.BaudRate     = baudrate;
    g_uart1_handle.Init.WordLength   = UART_WORDLENGTH_8B;
    g_uart1_handle.Init.StopBits     = UART_STOPBITS_1;
    g_uart1_handle.Init.Parity       = UART_PARITY_NONE;
    g_uart1_handle.Init.Mode         = UART_MODE_TX_RX;
    g_uart1_handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    g_uart1_handle.Init.OverSampling = UART_OVERSAMPLING_16;

    HAL_UART_Init(&g_uart1_handle);
}

/**
 * @brief       串口1底层初始化（由 HAL_UART_Init 回调）
 * @param       huart: UART 句柄
 * @retval      无
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef gpio_init_struct = {0};

    if (huart->Instance == USART_UX)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();               /* 使能 GPIOA 时钟 */
        __HAL_RCC_USART1_CLK_ENABLE();              /* 使能 USART1 时钟 */

        gpio_init_struct.Pin   = USART_TX_GPIO_PIN; /* PA9：复用推挽输出 */
        gpio_init_struct.Mode  = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull  = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(USART_TX_GPIO_PORT, &gpio_init_struct);

        gpio_init_struct.Pin  = USART_RX_GPIO_PIN;  /* PA10：上拉输入（本工程未使用接收） */
        gpio_init_struct.Mode = GPIO_MODE_INPUT;
        HAL_GPIO_Init(USART_RX_GPIO_PORT, &gpio_init_struct);
    }
}
