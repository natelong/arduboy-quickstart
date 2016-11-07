#ifndef EEP_H
#define EEP_H
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
typedef struct {
	uint8_t* adr;

} ST_EEP;


//---------------------------------------------------------------------------
void EepInit(void);
void EepSeek(uint16_t adr);

uint8_t   EepRead8(void);
uint16_t  EepRead16(void);
uint32_t  EepRead32(void);
void EepWrite8(uint8_t data);
void EepWrite16(uint16_t data);
void EepWrite32(uint32_t data);


#ifdef __cplusplus
}
#endif
#endif
