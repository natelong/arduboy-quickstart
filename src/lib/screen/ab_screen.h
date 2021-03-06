#pragma once

#include "../ab_common.h"

#define AB_OLED_WIDTH      128
#define AB_OLED_HEIGHT      64
#define AB_OLED_CHARWIDTH  (AB_OLED_WIDTH / AB_FONT_WIDTH) // 21
#define AB_OLED_CHARHEIGHT (AB_OLED_HEIGHT / AB_FONT_HEIGHT) // 8

typedef struct {
    uint8_t width;
    uint8_t height;
    const uint8_t* data;
} ab_Image;

void ab_screen_init(void);
void ab_screen_display(void);
void ab_screen_clear(void);

void ab_screen_setCursor(uint8_t x, uint8_t y);
void ab_screen_drawString(const char* s);
void ab_screen_drawNumber(uint32_t n);
void ab_screen_drawChar(uint8_t x, uint8_t y, char c);

void ab_screen_drawDot(uint8_t x, uint8_t y);
void ab_screen_drawTile(uint8_t x, uint8_t y, const ab_Image* img);
void ab_screen_drawImage(uint8_t x, uint8_t y, const ab_Image* img);
