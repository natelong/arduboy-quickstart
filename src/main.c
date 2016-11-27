#include "lib/ab.h"

int main(void) {

    ab_init();

    uint8_t note = 40;
    bool playing = false;
    uint8_t framesOn = 15;
    uint8_t framesOff = 1;
    uint8_t onCounter = 0;
    uint8_t offCounter = 0;

    for (;;) {
        ab_key_update();

        uint8_t pressed  = ab_key_getPressed();

        if (pressed & AB_KEY_A) {
            if (!playing) {
                playing = true;
                onCounter = framesOn;
            } else {
                playing = false;
                onCounter = 0;
                offCounter = 0;
                ab_sound_stopChannel(AB_CHANNEL_1);
            }
        }

        if (pressed & AB_KEY_U) note++;
        if (pressed & AB_KEY_D) note--;

        note = note % 64;

        if (playing) {
            if (onCounter > 0) {
                if (onCounter == 1) {
                    offCounter = framesOff;
                    ab_sound_stopChannel(AB_CHANNEL_1);
                }
                onCounter--;
            }
            else if (offCounter > 0) {
                if (offCounter == 1) {
                    onCounter = framesOn;
                    ab_sound_playNote(AB_CHANNEL_1, note);
                }
                offCounter--;
            }
        }

        ab_screen_setCursor(0, 0);
        ab_screen_drawString("Note: ");
        ab_screen_drawNumber(note);
        ab_screen_setCursor(0, 1);
        ab_screen_drawString("Playing: ");
        if (playing) {
            ab_screen_drawString("true");
        } else {
            ab_screen_drawString("false");
        }
        ab_frame();
    }

    return 0;
}
