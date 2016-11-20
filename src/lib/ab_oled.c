#include "ab.h"
#include "../res/font.h"

// SSD1306 OLED

PROGMEM static const uint8_t OledBootProgram[] = {
    0xD5,   // Set Display Clock Divisor 0xF0
    0xF0,
    0x8D,   // Charge Pump Setting 0x14
    0x14,
    0xA1,   // Set Segment Re-map (A0) | (b0001)
    0xC8,   // Set COM Output Scan Direction
    0x81,   // Set Contrast 0xCF
    0xCF,
    0xD9,   // Set Precharge 0xF1
    0xF1,
    0xAF,   // Display On
    0x20,   // Set display mode
    0x00,   // Horizontal addressing mode
};

static uint8_t oled[AB_OLED_WIDTH * AB_OLED_HEIGHT / 8];
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

uint8_t numlen(uint32_t n) {
    if (n < 10)          return  1;
    if (n < 100)         return  2;
    if (n < 1000)        return  3;
    if (n < 10000)       return  4;
    if (n < 100000)      return  5;
    if (n < 1000000)     return  6;
    if (n < 10000000)    return  7;
    if (n < 100000000)   return  8;
    if (n < 1000000000)  return  9;
    return 0;
}

void ab_oled_init(void) {
    ab_oled_clear();

    uint8_t dcMask  = (1 << 4);
    uint8_t csMask  = (1 << 6);
    uint8_t rstMask = (1 << 7);

    uint8_t sreg = SREG;
    cli(); // Protect from a scheduler and prevent transactionBegin
    DDRD |= dcMask;  // OLED_DC pin mode to output
    DDRD |= csMask;  // OLED_CS pin mode to output
    DDRD |= rstMask; // OLED_RST pin mode to output

    PORTD |= rstMask;  // VDD (3.3V) goes high at start
    ab_delay(1);       // lets just chill for a ms
    PORTD &= ~rstMask; // bring reset low.
    ab_delay(10);      // wait 10ms
    PORTD |= rstMask;  // bring out of reset

    // Command Mode
    PORTD |=  csMask;
    PORTD &= ~dcMask;
    PORTD &= ~csMask;

    // Send boot Program
    for(uint8_t i = 0; i < sizeof(OledBootProgram); i++) {
        ab_spi_transfer(__LPM(OledBootProgram + i));
    }

    // Data Mode
    PORTD |=  dcMask;
    PORTD &= ~csMask;

    SREG = sreg;
}

void ab_oled_display(void) {
    uint16_t i;

    for(i=0; i<sizeof(oled); i++) {
        ab_spi_transfer(oled[i]);
    }
}

void ab_oled_setCursor(uint8_t x, uint8_t y) {
    cursor_x = x;
    cursor_y = y;
}

void ab_oled_drawString(const char* s) {
    for(uint8_t i = 0; s[i] != '\0'; i++) {
        ab_oled_drawChar(cursor_x++, cursor_y, s[i]);
    }
}

void ab_oled_drawNumber(uint32_t n) {
    uint8_t l = numlen(n);
    char s[10];
    memset(&s, '\0', 10);
    uint8_t si = l - 1;

    if (n == 0) {
        ab_oled_drawChar(cursor_x++, cursor_y, '0');
        return;
    }

    while (n) {
        s[si] = '0' + (n % 10);
        n /= 10;
        si--;
    }

    for(uint8_t i = 0; s[i] != '\0'; i++) {
        ab_oled_drawChar(cursor_x++, cursor_y, s[i]);
    }
}

void ab_oled_drawChar(uint8_t x, uint8_t y, char c) {
    if(x >= AB_OLED_CHARWIDTH || y >= AB_OLED_CHARHEIGHT) return;

    for(uint8_t i = 0; i < AB_FONT_SIZE; i++) {
        oled[(x * AB_FONT_WIDTH + i) + (y * AB_OLED_WIDTH)] = __LPM(font + (c * AB_FONT_SIZE) + i);
    }
}

void ab_oled_drawDot(uint8_t x, uint8_t y) {
    oled[x + (y / 8) * AB_OLED_WIDTH] |= _BV(y % 8);
}

void ab_oled_clear(void) {
    uint16_t i;
    for(i = 0; i < sizeof(oled); i++) {
        oled[i] = 0x00;
    }
}

void ab_oled_drawBmp(int8_t sx, int8_t sy, uint8_t* p) {
    uint8_t* d  = (uint8_t*)p;
    uint8_t  cx = __LPM(d++);
    uint8_t  cy = __LPM(d++);

    uint8_t chr, mask;
    uint8_t x, y, b;

    for(y=0; y<cy; y++) {
        for(x=0; x<cx; x+=8) {
            if(sy + y < 0 || sy + y >= AB_OLED_HEIGHT) break;

            chr  = __LPM(d++);
            mask = 0x80;

            for(b=0; b<8; b++) {
                if(sx + x + b >= 0 && sx + x + b < AB_OLED_WIDTH) {
                    if(chr & mask) {
                        oled[(sx + x + b) + ((sy + y) / 8) * AB_OLED_WIDTH] |= _BV((sy + y) & 0x7);
                    }
                }

                mask >>=1;
            }
        }
    }
}
