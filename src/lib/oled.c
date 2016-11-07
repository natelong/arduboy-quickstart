#include <stdarg.h>
#include "Arduino_mini.h"
#include "SPI.h"
#include "oled.h"
#include "../res/font.h"

// SSD1306 OLED

//---------------------------------------------------------------------------
PROGMEM const uint8_t OledBootProgram[] = {
	0xD5,	// Set Display Clock Divisor 0xF0
	0xF0,
	0x8D,	// Charge Pump Setting 0x14
	0x14,
	0xA1,	// Set Segment Re-map (A0) | (b0001)
	0xC8,	// Set COM Output Scan Direction
	0x81,	// Set Contrast 0xCF
	0xCF,
	0xD9,	// Set Precharge 0xF1
	0xF1,
	0xAF,	// Display On
	0x20,	// Set display mode
	0x00,	// Horizontal addressing mode
};

//---------------------------------------------------------------------------
ST_OLED Oled;


//---------------------------------------------------------------------------
void OledInit(void)
{
	_Memset(&Oled, 0x00, sizeof(ST_OLED));


	spi_begin();
	spi_setClockDivider(SPI_CLOCK_DIV2);

	mini_pinMode(OLED_PIN_DC,  OUTPUT);
	mini_pinMode(OLED_PIN_CS,  OUTPUT);
	mini_pinMode(OLED_PIN_RST, OUTPUT);

	// VDD (3.3V) goes high at start, lets just chill for a ms
	mini_digitalWrite(OLED_PIN_RST, HIGH);
	delay(1);

	// bring reset low. wait 10ms
	mini_digitalWrite(OLED_PIN_RST, LOW);
	delay(10);

	// bring out of reset
	mini_digitalWrite(OLED_PIN_RST, HIGH);

	// setup the ports we need to talk to the OLED
	volatile uint8_t* pCs    = portOutputRegister(digitalPinToPort(OLED_PIN_CS));
	volatile uint8_t* pDc    = portOutputRegister(digitalPinToPort(OLED_PIN_DC));
	uint8_t   csMask = digitalPinToBitMask(OLED_PIN_CS);
	uint8_t   dcMask = digitalPinToBitMask(OLED_PIN_DC);

	// Command Mode
	*pCs |=  csMask;
	*pDc &= ~dcMask;
	*pCs &= ~csMask;

	// Send boot Program
	uint8_t i;

	for(i=0; i<sizeof(OledBootProgram); i++)
	{
		spi_transfer(__LPM(OledBootProgram + i));
	}

	// Data Mode
	*pDc |=  dcMask;
	*pCs &= ~csMask;
}
//---------------------------------------------------------------------------
void OledDisplay(void)
{
	uint16_t i;

	for(i=0; i<sizeof(Oled.buf); i++)
	{
		spi_transfer(Oled.buf[i]);
	}
}
//---------------------------------------------------------------------------
void OledDrawStr(uint8_t fx, uint8_t fy, const char* fmt, ...)
{
	char s[40];

	va_list ap;
	va_start(ap, fmt);
	_SprintfDo(s, fmt, ap);
	va_end(ap);


	uint8_t i;

	for(i=0; s[i] != '\0'; i++)
	{
		OledDrawChr(fx++, fy, s[i]);
	}
}
//---------------------------------------------------------------------------
void OledDrawChr(uint8_t fx, uint8_t fy, char chr)
{
	if(fx >= OLED_SCREEN_FONT_CX || fy >= OLED_SCREEN_FONT_CY)
	{
		return;
	}

	uint8_t x;

	for(x=0; x<OLED_FONT_SIZE; x++)
	{
		Oled.buf[(fx * OLED_FONT_CX + x) + (fy * OLED_SCREEN_CX)] = __LPM(font + (chr * OLED_FONT_SIZE) + x);
	}
}
//---------------------------------------------------------------------------
void OledDrawDot(uint8_t x, uint8_t y)
{
	Oled.buf[x + (y / 8) * OLED_SCREEN_CX] |= _BV(y % 8);
}
//---------------------------------------------------------------------------
void OledDrawCls(void)
{
//	uint16_t i;
//	for(i=0; i<sizeof(Oled.buf); i++) Oled.buf[i] = 0x00;

	asm volatile(
		"movw  r30, %0                \n\t"
		"eor __tmp_reg__, __tmp_reg__ \n\t"
		"loop:                        \n\t"
		"st Z+, __zero_reg__          \n\t"
		"st Z+, __zero_reg__          \n\t"
		"st Z+, __zero_reg__          \n\t"
		"st Z+, __zero_reg__          \n\t"
		"inc __tmp_reg__              \n\t"
		"brne loop                    \n\t"

		: : "r" (Oled.buf) : "r30","r31"
	);
}
//---------------------------------------------------------------------------
void OledDrawBmp(int8_t sx, int8_t sy, uint8_t* p)
{
	uint8_t* d  = (uint8_t*)p;
	uint8_t  cx = __LPM(d++);
	uint8_t  cy = __LPM(d++);

	uint8_t chr, mask;
	uint8_t x, y, b;

	for(y=0; y<cy; y++)
	{
		for(x=0; x<cx; x+=8)
		{
			if(sy + y < 0 || sy + y >= OLED_SCREEN_CY)
			{
				break;
			}

			chr  = __LPM(d++);
			mask = 0x80;

			for(b=0; b<8; b++)
			{
				if(sx + x + b >= 0 && sx + x + b < OLED_SCREEN_CX)
				{
					if(chr & mask)
					{
						Oled.buf[(sx + x + b) + ((sy + y) / 8) * OLED_SCREEN_CX] |= _BV((sy + y) & 0x7);
					}
				}

			 	mask >>=1;
			}
		}
	}
}
