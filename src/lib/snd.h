#pragma once

#include "common.h"

//---------------------------------------------------------------------------
#define SND_MAX_CHANNEL			2

enum {
	SND_OP_PLAY_NOTE = 0x90,
	SND_OP_STOP_NOTE = 0x80,
	SND_OP_RESTART   = 0xe0,
	SND_OP_STOP      = 0xf0,
};

//---------------------------------------------------------------------------
#define SND_PIN1 5			// PC6
#define SND_PIN2 13			// PC7

//---------------------------------------------------------------------------
typedef struct {
	volatile uint8_t* pPinPort;
	uint8_t   pinMask;
} ST_SND_CH;

typedef struct {
	// Score
	bool isScorePlay;
	bool isScoreCh[SND_MAX_CHANNEL];
	uint32_t  scoreCnt;
	uint32_t  scoreFreqCnt;
	uint8_t*  pScoreStart;
	uint8_t*  pScoreCur;

	// Tone
	bool isTonePlay;
	uint32_t  toneCnt;

	// Pin
	ST_SND_CH ch[SND_MAX_CHANNEL];

} ST_SND;


//---------------------------------------------------------------------------
void SndInit(void);

void SndPlayScore(const uint8_t* p);
void SndStopScore(void);
void SndStepScore(void);

void SndPlayNote(uint8_t ch, uint8_t note);
void SndStopNote(uint8_t ch);
void SndPlayTone(uint16_t freq, uint32_t duration);
void SndStopTone(void);

void SndStartTimerCh0(void);
void SndStartTimerCh(uint8_t ch, uint32_t freq);
void SndStopTimer(uint8_t ch);
