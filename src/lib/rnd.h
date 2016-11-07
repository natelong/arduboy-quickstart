#ifndef RND_H
#define RND_H
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void RndInit(void);
void RndInitSeed(uint32_t num);

uint32_t  Rnd32(void);
uint32_t  Rnd(uint32_t num);


#ifdef __cplusplus
}
#endif
#endif
