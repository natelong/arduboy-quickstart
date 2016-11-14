#include "Arduino_mini.h"

#include "lib/ab.h"

typedef struct {
    uint32_t score;
} SaveData;

int main(void) {
    mini_init();

    ab_init();

    SaveData save;

    ab_eeprom_read(&save, sizeof(SaveData));

    uint8_t num = ab_random();

    for (;;) {
        if(ab_loopStart() == false) continue;

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

        ab_oled_drawStr(0, 0, "Arduino Mini: %d", save.score);
        ab_oled_drawStr(0, 1, "CPU: %d", FrameGetCpuPercent());
        ab_oled_drawStr(0, 2, "Random: %d", num);
        ab_loopEnd();
    }

    return 0;
}

