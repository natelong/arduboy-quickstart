#include <stdarg.h>
#include "Arduino_mini.h"
#include "oled.h"
#include "../res/font.h"

// SSD1306 OLED

#define SPI_CLOCK_DIV2   0x04
#define SPI_CLOCK_MASK   0x03 // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK 0x01 // SPI2X = bit 0 on SPSR

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

inline static void spi_transfer(uint8_t data) {
    SPDR = data;
    asm volatile("nop");
    while (!(SPSR & _BV(SPIF))) ; // wait
}

void ab_oled_init(void) {
    _Memset(&oled, 0x00, sizeof(oled));

    uint8_t sreg = SREG;
    cli(); // Protect from a scheduler and prevent transactionBegin
    {
        if(!(*mini_SPI_SS.mode & mini_SPI_SS.mask)) {
            mini_setPinHigh(&mini_SPI_SS);
        }

        mini_pinModeOutput(&mini_SPI_SS);

        SPCR |= _BV(MSTR);
        SPCR |= _BV(SPE);

        mini_pinModeOutput(&mini_SPI_SCK);
        mini_pinModeOutput(&mini_SPI_MOSI);
    }
    SREG = sreg;

    SPCR = (SPCR & ~SPI_CLOCK_MASK) | (SPI_CLOCK_DIV2 & SPI_CLOCK_MASK);
    SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((SPI_CLOCK_DIV2 >> 2) & SPI_2XCLOCK_MASK);

    mini_pinModeOutput(&mini_OLED_DC);
    mini_pinModeOutput(&mini_OLED_CS);
    mini_pinModeOutput(&mini_OLED_RST);

    // VDD (3.3V) goes high at start, lets just chill for a ms
    mini_setPinHigh(&mini_OLED_RST);
    mini_delay(1);

    // bring reset low. wait 10ms
    mini_setPinLow(&mini_OLED_RST);
    mini_delay(10);

    // bring out of reset
    mini_setPinHigh(&mini_OLED_RST);

    // setup the ports we need to talk to the OLED
    volatile uint8_t* pCs = mini_OLED_CS.out;
    volatile uint8_t* pDc = mini_OLED_DC.out;

    // Command Mode
    *pCs |=  mini_OLED_CS.mask;
    *pDc &= ~mini_OLED_DC.mask;
    *pCs &= ~mini_OLED_CS.mask;

    // Send boot Program
    uint8_t i;

    for(i=0; i<sizeof(OledBootProgram); i++) {
        spi_transfer(__LPM(OledBootProgram + i));
    }

    // Data Mode
    *pDc |=  mini_OLED_DC.mask;
    *pCs &= ~mini_OLED_CS.mask;
}

void ab_oled_display(void) {
    uint16_t i;

    for(i=0; i<sizeof(oled); i++) {
        spi_transfer(oled[i]);
    }
}

void ab_oled_drawStr(uint8_t fx, uint8_t fy, const char* fmt, ...) {
    char s[40];

    va_list ap;
    va_start(ap, fmt);
    _SprintfDo(s, fmt, ap);
    va_end(ap);

    uint8_t i;

    for(i=0; s[i] != '\0'; i++) {
        ab_oled_drawChr(fx++, fy, s[i]);
    }
}

void ab_oled_drawChr(uint8_t fx, uint8_t fy, char chr) {
    if(fx >= AB_OLED_CHARWIDTH || fy >= AB_OLED_CHARHEIGHT) {
        return;
    }

    uint8_t x;

    for(x=0; x<AB_FONT_SIZE; x++) {
        oled[(fx * AB_FONT_WIDTH + x) + (fy * AB_OLED_WIDTH)] = __LPM(font + (chr * AB_FONT_SIZE) + x);
    }
}

void ab_oled_drawDot(uint8_t x, uint8_t y) {
    oled[x + (y / 8) * AB_OLED_WIDTH] |= _BV(y % 8);
}

void ab_oled_clear(void) {
    //  uint16_t i;
    //  for(i=0; i<sizeof(oled); i++) oled[i] = 0x00;
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

        : : "r" (oled) : "r30","r31"
    );
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
