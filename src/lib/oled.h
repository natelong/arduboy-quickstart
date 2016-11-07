#pragma once

#include "common.h"

//---------------------------------------------------------------------------
#define OLED_FONT_CX			6
#define OLED_FONT_CY			8
#define OLED_FONT_SIZE			5

#define OLED_SCREEN_CX			128
#define OLED_SCREEN_CY			64
#define OLED_SCREEN_FONT_CX		(OLED_SCREEN_CX / OLED_FONT_CX)		// 21
#define OLED_SCREEN_FONT_CY		(OLED_SCREEN_CY / OLED_FONT_CY)		// 8

//---------------------------------------------------------------------------
#if defined(ARDUBOY_10)

	#define OLED_PIN_CS			12
	#define OLED_PIN_DC			4
	#define OLED_PIN_RST		6

#elif defined(ARDUBOY_DEVKIT)

	#define OLED_PIN_CS			6
	#define OLED_PIN_DC			4
	#define OLED_PIN_RST		12

#endif

//---------------------------------------------------------------------------
typedef struct {
	uint8_t buf[OLED_SCREEN_CX * (OLED_SCREEN_CY / 8)];

} ST_OLED;


//---------------------------------------------------------------------------
void OledInit(void);
void OledDisplay(void);

void OledDrawStr(uint8_t fx, uint8_t fy, const char* fmt, ...);
void OledDrawChr(uint8_t fx, uint8_t fy, char chr);
void OledDrawDot(uint8_t x, uint8_t y);
void OledDrawCls(void);

void OledDrawBmp(int8_t sx, int8_t sy, uint8_t* p);
