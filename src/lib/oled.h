#pragma once

#include "common.h"

#define AB_OLED_WIDTH      128
#define AB_OLED_HEIGHT      64
#define AB_OLED_CHARWIDTH  (AB_OLED_WIDTH / AB_FONT_WIDTH) // 21
#define AB_OLED_CHARHEIGHT (AB_OLED_HEIGHT / AB_FONT_HEIGHT) // 8

void ab_oled_init(void);
void ab_oled_display(void);
void ab_oled_clear(void);

void ab_oled_drawStr(uint8_t fx, uint8_t fy, const char* fmt, ...);
void ab_oled_drawChr(uint8_t fx, uint8_t fy, char chr);
void ab_oled_drawDot(uint8_t x, uint8_t y);
void ab_oled_drawBmp(int8_t sx, int8_t sy, uint8_t* p);
