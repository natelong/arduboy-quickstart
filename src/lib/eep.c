#include <avr/eeprom.h>
#include "eep.h"

//---------------------------------------------------------------------------
ST_EEP Eep;


//---------------------------------------------------------------------------
void EepInit(void)
{
	_Memset(&Eep, 0x00, sizeof(ST_EEP));
}
//---------------------------------------------------------------------------
void EepSeek(uint16_t adr)
{
	Eep.adr = (uint8_t*)adr;
}
//---------------------------------------------------------------------------
uint8_t EepRead8(void)
{
	eeprom_busy_wait();
	return eeprom_read_byte(Eep.adr++);
}
//---------------------------------------------------------------------------
uint16_t EepRead16(void)
{
	uint16_t ret = 0;

	ret |= EepRead8();
	ret |= EepRead8() << 8;

	return ret;
}
//---------------------------------------------------------------------------
uint32_t EepRead32(void)
{
	uint32_t ret = 0;

	ret |= (uint32_t)EepRead8();
	ret |= (uint32_t)EepRead8() <<  8;
	ret |= (uint32_t)EepRead8() << 16;
	ret |= (uint32_t)EepRead8() << 24;

	return ret;
}
//---------------------------------------------------------------------------
void EepWrite8(uint8_t data)
{
	eeprom_busy_wait();
	eeprom_write_byte(Eep.adr++, data);
}
//---------------------------------------------------------------------------
void EepWrite16(uint16_t data)
{
	EepWrite8(data & 0xff);
	EepWrite8(data >> 8);
}
//---------------------------------------------------------------------------
void EepWrite32(uint32_t data)
{
	EepWrite8((data      ) & 0xff);
	EepWrite8((data >>  8) & 0xff);
	EepWrite8((data >> 16) & 0xff);
	EepWrite8((data >> 24) & 0xff);
}
