#include "ab_screen.h"
#include "../core/ab_core.h"
#include "../../res/font.h"

#define SPI_CLOCK_DIV2   0x04
#define SPI_CLOCK_MASK   0x03 // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK 0x01 // SPI2X = bit 0 on SPSR

static void spi_init(void) {
    uint8_t sreg = SREG;
    cli(); // Protect from a scheduler and prevent transactionBegin

    if (!(DDRB & (1 << 0))) {
        PORTB |= (1 << 0); // set SS to high
    }
    DDRB |= (1 << 0); // set SS mode to output

    SPCR |= _BV(MSTR);
    SPCR |= _BV(SPE);

    DDRB  |= (1 << 1); // set SCK mode to output
    DDRB  |= (1 << 2); // set MOSI mode to output

    SREG = sreg;

    SPCR = (SPCR & ~SPI_CLOCK_MASK) | (SPI_CLOCK_DIV2 & SPI_CLOCK_MASK);
    SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((SPI_CLOCK_DIV2 >> 2) & SPI_2XCLOCK_MASK);
}

static inline void spi_transfer(uint8_t data) {
    SPDR = data;
    asm volatile("nop");
    while (!(SPSR & _BV(SPIF))) ; // wait
}

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

void ab_screen_init(void) {
    spi_init();
    ab_screen_clear();

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
        spi_transfer(__LPM(OledBootProgram + i));
    }

    // Data Mode
    PORTD |=  dcMask;
    PORTD &= ~csMask;

    SREG = sreg;
}

void ab_screen_display(void) {
    uint16_t i;

    for(i=0; i<sizeof(oled); i++) {
        spi_transfer(oled[i]);
    }
}

void ab_screen_setCursor(uint8_t x, uint8_t y) {
    cursor_x = x;
    cursor_y = y;
}

void ab_screen_drawString(const char* s) {
    for(uint8_t i = 0; s[i] != '\0'; i++) {
        ab_screen_drawChar(cursor_x++, cursor_y, s[i]);
    }
}

void ab_screen_drawNumber(uint32_t n) {
    uint8_t l = numlen(n);
    char s[10];
    memset(&s, '\0', 10);
    uint8_t si = l - 1;

    if (n == 0) {
        ab_screen_drawChar(cursor_x++, cursor_y, '0');
        return;
    }

    while (n) {
        s[si] = '0' + (n % 10);
        n /= 10;
        si--;
    }

    for(uint8_t i = 0; s[i] != '\0'; i++) {
        ab_screen_drawChar(cursor_x++, cursor_y, s[i]);
    }
}

void ab_screen_drawChar(uint8_t x, uint8_t y, char c) {
    if(x >= AB_OLED_CHARWIDTH || y >= AB_OLED_CHARHEIGHT) return;

    for(uint8_t i = 0; i < AB_FONT_SIZE; i++) {
        oled[(x * AB_FONT_WIDTH + i) + (y * AB_OLED_WIDTH)] = pgm_read_byte(font + (c * AB_FONT_SIZE) + i);
    }
}

void ab_screen_drawDot(uint8_t x, uint8_t y) {
    oled[x + (y / 8) * AB_OLED_WIDTH] |= _BV(y % 8);
}

void ab_screen_clear(void) {
    uint16_t i;
    for(i = 0; i < sizeof(oled); i++) {
        oled[i] = 0x00;
    }
}

void ab_screen_drawTile(int8_t cx, int8_t cy, const ab_Image* img) {
    uint8_t x, y;
    uint8_t w = pgm_read_byte(&img->width);
    uint8_t h = pgm_read_byte(&img->height);
    const uint8_t* data = pgm_read_byte(&img->data);
    uint16_t offset = 0;

    if (cy % 8 != 0) return;
    if (cx + w > AB_OLED_WIDTH) return;
    if (cy + h > AB_OLED_HEIGHT) return;

    for(y = 0; y < h; y += 8) {
        for(x = 0; x < w; x++) {
            oled[(cx + x) + ((cy + y) / 8) * AB_OLED_WIDTH] = pgm_read_byte(data + offset);
            offset++;
        }
    }
}

void ab_screen_drawImage(int8_t cx, int8_t cy, const ab_Image* img) {
    uint8_t x, y;
    uint8_t w = pgm_read_byte(&img->width);
    uint8_t h = pgm_read_byte(&img->height);
    const uint8_t* data = pgm_read_word(&img->data);
    uint16_t offset = 0;
    uint8_t yoff = cy % 8;
    uint8_t val;

    if (yoff == 0) {
        ab_screen_drawTile(cx, cy, img); // switch to faster draw
    } else {
        offset = 0;
        for(y = 0; y < (h / 8) + 1; ++y) {
            for(x = 0; x < w; x++) {
                val = 0;
                if (y < (h / 8)) val |= pgm_read_byte(data + offset) << yoff;
                if (y > 0)       val |= pgm_read_byte(data + offset - w) >> (8 - yoff);

                oled[(cx + x) + (cy / 8 + y) * AB_OLED_WIDTH] = val;
                offset++;
            }
        }
    }
}
