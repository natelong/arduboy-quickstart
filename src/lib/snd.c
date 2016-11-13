#include "Arduino_mini.h"
#include <avr/power.h>
#include "snd.h"
#include "oled.h"

PROGMEM const unsigned int SndMidiNoteFreq[128] = {
    16,17,18,19,21,22,23,24,26,28,29,31,33,35,37,39,41,44,46,49,52,55,58,62,65,
    69,73,78,82,87,92,98,104,110,117,123,131,139,147,156,165,175,185,196,208,220,
    233,247,262,277,294,311,330,349,370,392,415,440,466,494,523,554,587,622,659,
    698,740,784,831,880,932,988,1047,1109,1175,1245,1319,1397,1480,1568,1661,1760,
    1865,1976,2093,2217,2349,2489,2637,2794,2960,3136,3322,3520,3729,3951,4186,
    4435,4699,4978,5274,5588,5920,6272,6645,7040,7459,7902,8372,8870,9397,9956,
    10548,11175,11840,12544,13290,14080,14917,15804,16744,17740,18795,19912,21096,
    22351,23680,25088
};

ST_SND Snd;

void SndInit(void) {
    _Memset(&Snd, 0x00, sizeof(ST_SND));

    mini_pinModeOutput(&mini_SND_1);
    Snd.ch[0] = &mini_SND_1;

    mini_pinModeOutput(&mini_SND_2);
    Snd.ch[1] = &mini_SND_2;

    TCCR3A = 0;
    TCCR3B = 0;
    TCCR1A = 0;
    TCCR1B = 0;

    mini_bitSet(TCCR3B, WGM32);
    mini_bitSet(TCCR3B, CS30);
    mini_bitSet(TCCR1B, WGM12);
    mini_bitSet(TCCR1B, CS10);

    power_timer3_enable();
    power_timer1_enable();
}

void SndPlayScore(const uint8_t* p) {
    Snd.pScoreStart  = (uint8_t*)p;
    Snd.pScoreCur    = (uint8_t*)p;
    Snd.scoreCnt     = 1;
    Snd.scoreFreqCnt = 0;
    Snd.isScoreCh[0] = false;
    Snd.isScoreCh[1] = false;
    Snd.isScorePlay  = true;

    SndStartTimerCh0();
}

void SndStopScore(void) {
    SndStopTimer(0);
    SndStopTimer(1);

    Snd.isScorePlay = false;
}

void SndStepScore(void) {
    for(;;) {
        uint8_t cmd = __LPM(Snd.pScoreCur++);

        uint8_t op  = cmd & 0xf0;
        uint8_t ch  = cmd & 0x0f;

        switch(op) {
        case SND_OP_PLAY_NOTE:
            SndPlayNote(ch, __LPM(Snd.pScoreCur++));
            break;

        case SND_OP_STOP_NOTE:
            SndStopNote(ch);
            break;

        case SND_OP_RESTART:
            Snd.pScoreCur = Snd.pScoreStart;
            break;

        case SND_OP_STOP:
            SndStopScore();
            return;

        default:
            if(op < 0x80) {
                // wait count in msec.
                uint16_t duration = ((uint16_t)cmd << 8) | __LPM(Snd.pScoreCur++);
                Snd.scoreCnt = ((uint32_t)Snd.scoreFreqCnt * duration + 500) / 1000;

                if(Snd.scoreCnt == 0) Snd.scoreCnt = 1;
                return;
            }

            _SystemError("[Snd %x %x]", cmd, Snd.pScoreCur - Snd.pScoreStart);
        }
    } // for(;;)
}

void SndPlayNote(uint8_t ch, uint8_t note) {
    if(ch == 1 && Snd.isTonePlay == true) return;

    uint16_t freq = __LPM_word(SndMidiNoteFreq + note);

    if(ch == 0) Snd.scoreFreqCnt = freq;

    Snd.isScoreCh[ch] = true;
    SndStartTimerCh(ch, F_CPU / freq);
}

void SndStopNote(uint8_t ch) {
    Snd.isScoreCh[ch] = false;

    // TODO: Check this
    // *Snd.ch[ch].pPinPort &= ~Snd.ch[ch].pinMask;
    *Snd.ch[ch]->out &= ~Snd.ch[ch]->mask;
    // mini_setPinLow(Snd.ch[ch]);
}

void SndPlayTone(uint16_t freq, uint32_t duration) {
    if(Snd.isTonePlay == true) return;

    Snd.isTonePlay = true;

    uint32_t cnt = 2 * freq * duration / 1000;

    if(cnt == 0) return;
    Snd.toneCnt = cnt;


    SndStartTimerCh(1, F_CPU / freq / 2);
}

void SndStopTone(void) {
    SndStopTimer(1);
    Snd.isTonePlay = false;
}

void SndStartTimerCh0(void) {
    TCCR3B = (TCCR3B & 0xf8) | 0x01;
    OCR3A  = 0xffff;

    mini_bitSet(TIMSK3, OCIE3A);
}

void SndStartTimerCh(uint8_t ch, uint32_t freq) {
    // timer ck/1
    uint32_t ocr = freq;
    uint8_t  pre = 0x01;

    if(ocr > 0xffff) {
        // ck/64
        ocr /= 64;
        pre  = 0x03;
    }
    ocr--;

    if(ch == 0) {
        TCCR3B = (TCCR3B & 0xf8) | pre;
        OCR3A  = ocr;
        mini_bitSet(TIMSK3, OCIE3A);
    } else {
        TCCR1B = (TCCR1B & 0xf8) | pre;
        OCR1A  = ocr;
        mini_bitSet(TIMSK1, OCIE1A);
    }
}

void SndStopTimer(uint8_t ch) {
    if(ch == 0) {
        TIMSK3 &= ~(1 << OCIE3A);
        // TODO: test this
        // *Snd.ch[0].pPinPort &= ~Snd.ch[0].pinMask;
        *Snd.ch[0]->out &= ~Snd.ch[0]->mask;
        // mini_setPinLow(Snd.ch[0]);
    } else {
        TIMSK1 &= ~(1 << OCIE1A);
        // TODO: test this
        // *Snd.ch[1].pPinPort &= ~Snd.ch[1].pinMask;
        *Snd.ch[1]->out &= ~Snd.ch[1]->mask;
        // mini_setPinLow(Snd.ch[1]);
    }
}

// TIMER 3 ch0
ISR(TIMER3_COMPA_vect) {
    if(Snd.isScoreCh[0] == true) *Snd.ch[0]->out ^= Snd.ch[0]->mask;
    Snd.scoreCnt--;
    if(Snd.scoreCnt == 0) SndStepScore();
}

// TIMER 1 ch1
ISR(TIMER1_COMPA_vect) {
    *Snd.ch[1]->out ^= Snd.ch[1]->mask;
    if(Snd.isTonePlay == false) return;
    if(--Snd.toneCnt == 0) SndStopTone();
}
