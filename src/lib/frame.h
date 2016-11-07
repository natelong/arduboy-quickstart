#pragma once

#include <stdint.h>
#include "common.h"

//---------------------------------------------------------------------------
typedef struct {
    uint8_t   rate;
    uint16_t  count;
    uint32_t  nextStart;
    uint32_t  lastStart;
    uint8_t   lastDurationMs;
    uint8_t   eachMillis;
    bool isPostRender;

    uint16_t  cpuMaxPercent;
    uint8_t   cnt;
} ST_FRAME;


//---------------------------------------------------------------------------
void FrameInit(void);
void FrameSetRate(uint8_t rate);
uint16_t  FrameGetCpuPercent(void);
uint16_t  FrameGetCpuPercentMax(void);
uint8_t   FrameGetCnt(void);

bool FrameLoop(void);
