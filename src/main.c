#include "lib/ab.h"

typedef struct {
    uint32_t score;
} SaveData;

#define FRAME_COUNT 32
static uint8_t  rbuf_frames[FRAME_COUNT];
static uint8_t  rbuf_cursor = 0;
static uint32_t rbuf_total = 0;

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Color;

int main(void) {
    SaveData save;

    for (uint8_t i = 0; i < FRAME_COUNT; i++) {
        rbuf_frames[i] = 0;
    }

    ab_init();
    {
        ab_key_update();
        uint8_t pressed  = ab_key_getPressed();
        if (pressed & AB_KEY_U) {
            ab_setLED(0, 0, 255);
            while (true) {}
            return 1;
        }
    }

    ab_eeprom_read(&save, sizeof(SaveData));

    uint8_t num = ab_random();

    uint32_t last = 0;
    Color colors[] = {
        {255, 255, 255},
        {  0,   0,   0},
        {255,   0,   0},
        {  0, 255,   0},
        {  0,   0, 255}
    };

    for (;;) {
        uint32_t now = ab_millis();
        uint8_t delta = (uint8_t)(now - last);
        rbuf_total -= rbuf_frames[rbuf_cursor];
        rbuf_total += delta;
        rbuf_frames[rbuf_cursor] = delta;
        rbuf_cursor = (rbuf_cursor+1) % FRAME_COUNT;
        last = now;
        ab_key_update();

        uint8_t pressed  = ab_key_getPressed();
        uint8_t released = ab_key_getReleased();

        if (pressed & AB_KEY_U) save.score++;
        if (pressed & AB_KEY_D) ab_eeprom_write(&save, sizeof(SaveData));
        if (pressed & AB_KEY_L) {
            Color c = colors[ab_random() % (sizeof(colors) / sizeof(Color))];
            ab_setLED(c.r, c.b, c.g);
        }
        if (pressed & AB_KEY_R) num = ab_random();

        if (pressed & AB_KEY_A)  ab_sound_playNote(&ab_Channel_1, 64);
        if (released & AB_KEY_A) ab_sound_stopNote(&ab_Channel_1);

        if (pressed & AB_KEY_B)  ab_sound_playNote(&ab_Channel_2, 32);
        if (released & AB_KEY_B) ab_sound_stopNote(&ab_Channel_2);

        if ((ab_key_getCurrent() & AB_KEY_ALL) == AB_KEY_ALL) ab_reset();

        ab_oled_setCursor(0, 0);
        ab_oled_drawString("Arduino Mini: ");
        ab_oled_drawNumber(save.score);
        ab_oled_setCursor(0, 1);
        ab_oled_drawString("Random: ");
        ab_oled_drawNumber(num);
        ab_oled_setCursor(0, 2);
        ab_oled_drawString("Frame: ");
        ab_oled_drawNumber(rbuf_total / FRAME_COUNT);


        // ab_debug();

        ab_frame();
    }

    return 0;
}

