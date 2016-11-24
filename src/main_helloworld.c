#include "lib/ab.h"

int main(void) {

    ab_init();

    for (;;) {
        ab_oled_setCursor(0, 0);
        ab_oled_drawString("Hello, World!");
        ab_frame();
    }

    return 0;
}
