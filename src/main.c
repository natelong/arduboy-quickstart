#include "Arduino_mini.h"

#include "lib/ab.h"

typedef struct {
    uint32_t score;
} SaveData;

#define FRAME_COUNT 32
static uint8_t  rbuf_frames[FRAME_COUNT];
static uint8_t  rbuf_cursor = 0;
static uint32_t rbuf_total = 0;

int main(void) {
    SaveData save;

    for (uint8_t i = 0; i < FRAME_COUNT; i++) {
        rbuf_frames[i] = 0;
    }

    mini_init();
    ab_init();

    ab_eeprom_read(&save, sizeof(SaveData));

    uint8_t num = ab_random();

    uint32_t last = 0;
    for (;;) {
        uint32_t now = mini_millis();
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
        if (pressed & AB_KEY_L) // placeholder
        if (pressed & AB_KEY_R) num = ab_random();

        if (pressed & AB_KEY_A)  ab_sound_playNote(&ab_Channel_1, 64);
        if (released & AB_KEY_A) ab_sound_stopNote(&ab_Channel_1);

        if (pressed & AB_KEY_B)  ab_sound_playNote(&ab_Channel_2, 32);
        if (released & AB_KEY_B) ab_sound_stopNote(&ab_Channel_2);

        ab_oled_drawStr( 0, 0, "Arduino Mini: ");
        ab_oled_drawNum(14, 0, save.score);
        ab_oled_drawStr( 0, 2, "Random: ");
        ab_oled_drawNum( 8, 2, num);
        ab_oled_drawStr( 0, 3, "Frame: ");
        ab_oled_drawNum( 7, 3, rbuf_total / FRAME_COUNT);
        ab_frame();
    }

    return 0;
}

