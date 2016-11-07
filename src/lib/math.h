#ifndef MATH_H
#define MATH_H
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
void MathInit(void);
int8_t   MathSin(uint8_t deg);
int8_t   MathCos(uint8_t deg);


#ifdef __cplusplus
}
#endif
#endif
