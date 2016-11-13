#include "Arduino_mini.h"

#include "lib/ab.h"

int main(void) {
    mini_init();

    ab_init();

    uint8_t x = 0;
    uint8_t y = 0;

    for (;;) {
        if(ab_loopStart() == false) continue;

        uint8_t trg = KeyGetTrg();
        if (trg & KEY_U) y--;
        if (trg & KEY_D) y++;
        if (trg & KEY_L) x--;
        if (trg & KEY_R) x++;

        ab_oled_drawStr(x, y, "Arduino Mini");
        ab_loopEnd();
    }

    return 0;
}
