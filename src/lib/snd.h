#pragma once

#include "common.h"
#include "Arduino_mini.h"

typedef struct {
    volatile uint8_t*  mask;
    volatile uint16_t* source;
} ab_Channel;

extern const ab_Channel ab_Channel_1;
extern const ab_Channel ab_Channel_2;

void ab_sound_init(void);
void ab_sound_playNote(const ab_Channel* channel, uint16_t frequency);
void ab_sound_stopNote(const ab_Channel* channel);
