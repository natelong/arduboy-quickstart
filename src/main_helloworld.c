#include "lib/ab.h"

int main(void) {

    ab_init();

    for (;;) {
        ab_screen_setCursor(0, 0);
        ab_screen_drawString("Hello, World!");
        ab_frame();
    }

    return 0;
}
