#ifndef __OLED_H
#define __OLED_H

#include "stm32f1xx_hal.h"

#define OLED_ADDR         0x78
#define OLED_WIDTH        128
#define OLED_HEIGHT       64

void OLED_Init(void);
void OLED_Clear(void);
void OLED_Refresh(void);
void OLED_RefreshControl(uint8_t en);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr);
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr);
void OLED_ShowString_Fast(uint8_t x, uint8_t y, uint8_t *chr);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len);
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t);
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

#endif
