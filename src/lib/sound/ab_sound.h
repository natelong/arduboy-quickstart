#pragma once

#include "../ab_common.h"

extern const uint8_t AB_CHANNEL_1;
extern const uint8_t AB_CHANNEL_2;

void ab_sound_init(void);
void ab_sound_playNote(uint8_t channel, uint8_t note);
void ab_sound_playTone(uint8_t channel, uint16_t frequency);
void ab_sound_stopChannel(uint8_t channel);
