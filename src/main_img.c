#include "lib/ab.h"
#include "res/image.h"

int main(void) {
    ab_init();

    uint8_t x = 0;
    uint8_t y = 0;

    for (;;) {
        ab_key_update();

        uint8_t pressed  = ab_key_getPressed();

        if (pressed & AB_KEY_U) if (y > 0) y--;
        if (pressed & AB_KEY_D) if (y < AB_OLED_HEIGHT-1) y++;
        if (pressed & AB_KEY_L) if (x > 0) x--;
        if (pressed & AB_KEY_R) if (x < AB_OLED_WIDTH-1) x++;

        ab_screen_drawImage(x, y, &img_stephanie_png);

        // ab_debug();

        ab_frame();
    }

    return 0;
}

