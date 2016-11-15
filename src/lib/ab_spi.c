#include "ab.h"

#define SPI_CLOCK_DIV2   0x04
#define SPI_CLOCK_MASK   0x03 // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK 0x01 // SPI2X = bit 0 on SPSR

void ab_spi_init(void) {
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

inline void ab_spi_transfer(uint8_t data) {
    SPDR = data;
    asm volatile("nop");
    while (!(SPSR & _BV(SPIF))) ; // wait
}