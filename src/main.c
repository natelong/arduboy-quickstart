#include "Arduino_mini.h"

#include "lib/ab.h"

typedef struct {
    uint32_t score;
} SaveData;

int main(void) {
    mini_init();

    ab_init();

    SaveData save;

    ab_eep_read(&save, sizeof(SaveData));

    uint8_t x = 0;

    for (;;) {
        if(ab_loopStart() == false) continue;

        uint8_t trg = KeyGetTrg();
        uint8_t off = KeyGetOff();

        if (trg & KEY_U) save.score++;
        if (trg & KEY_D) ab_eep_write(&save, sizeof(SaveData));
        if (trg & KEY_L) x--;
        if (trg & KEY_R) x++;

        if (trg & KEY_A) ab_sound_playNote(&ab_Channel_1, 64);
        if (off & KEY_A) ab_sound_stopNote(&ab_Channel_1);

        if (trg & KEY_B) ab_sound_playNote(&ab_Channel_2, 32);
        if (off & KEY_B) ab_sound_stopNote(&ab_Channel_2);

        ab_oled_drawStr(x, 0, "Arduino Mini: %d", save.score);
        ab_loopEnd();
    }

    return 0;
}

