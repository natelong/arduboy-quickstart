#include "rnd.h"
#include "tinymt32.h"


//---------------------------------------------------------------------------
tinymt32_t MathTinyMt;

//---------------------------------------------------------------------------
void RndInit(void)
{
	tinymt32_init(&MathTinyMt, 0xF67C129B);
}
//---------------------------------------------------------------------------
void RndInitSeed(uint32_t num)
{
	tinymt32_init(&MathTinyMt, num);
}
//---------------------------------------------------------------------------
uint32_t Rnd32(void)
{
	return tinymt32_generate_uint32(&MathTinyMt);
}
//---------------------------------------------------------------------------
uint32_t Rnd(uint32_t num)
{
	int32_t ret = Rnd32() % num;

	if(ret < 0)
	{
		ret *= -1;
	}

	return ret;
}
