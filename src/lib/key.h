#pragma once

#include <Arduino_mini.h>
#include "common.h"

//---------------------------------------------------------------------------
#define KEY_REPEAT_CNT			10

//---------------------------------------------------------------------------

	#define KEY_PIN_L PIN_A2 // PF5
	#define KEY_PIN_R PIN_A1 // PF6
	#define KEY_PIN_U PIN_A0 // PF7
	#define KEY_PIN_D PIN_A3 // PF4
	#define KEY_PIN_A 7      // PE6
	#define KEY_PIN_B 8		 // PB4

//---------------------------------------------------------------------------
enum {

#if defined(ARDUBOY_10)

	KEY_A = _BV(3),
	KEY_B = _BV(2),
	KEY_L = _BV(5),
	KEY_R = _BV(6),
	KEY_U = _BV(7),
	KEY_D = _BV(4),

#elif defined(ARDUBOY_DEVKIT)

	KEY_A = _BV(1),
	KEY_B = _BV(0),
	KEY_R = _BV(2),
	KEY_U = _BV(4),
	KEY_L = _BV(5),
	KEY_D = _BV(6),

#endif

	DPAD  = (KEY_U | KEY_D | KEY_L | KEY_R),
};

//---------------------------------------------------------------------------
typedef struct {
	uint8_t cnt;
	uint8_t trg;
	uint8_t off;
	uint8_t rep;
	int8_t repCnt;

} ST_KEY;

//---------------------------------------------------------------------------
void KeyInit(void);
void KeyLoop(void);

uint8_t   KeyGetCnt(void);
uint8_t   KeyGetTrg(void);
uint8_t   KeyGetOff(void);
uint8_t   KeyGetRep(void);
