#include "common.h"
#include "oled.h"

#include <stdint.h>

//---------------------------------------------------------------------------
void _Memset(void* s, uint8_t c, int16_t size)
{
	volatile char* s1 = (char*)s;

	if(size == 0)
	{
		goto End;
	}

	size++;
	while(--size > 0)
	{
		*s1++ = c;
	}

End: ;
}
//---------------------------------------------------------------------------
char* _Sprintf(char* buf, const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	_SprintfDo(buf, fmt, ap);
	va_end(ap);

	return buf;
}
//---------------------------------------------------------------------------
void _SprintfDo(char* str, const char* fmt, va_list ap)
{
	int16_t   val;
	char* val2;
	char  val3;

	char c;
	int16_t  col = 0;
	char colChr = ' ';
	bool isCol;

	for(;;)
	{
		c = *fmt++;

		if(c == '\0')
		{
			*str++ = '\0';
			return;
		}

		if(c != '%')
		{
			*str++ = c;
			continue;
		}

		c = *fmt++;
		if(c == '0')
		{
			colChr = '0';
			c = *fmt++;
		}

		if(_IsDigit(c) == true)
		{
			col   = c - '0';
			isCol = true;
			c = *fmt++;
		}
		else
		{
			isCol = false;
		}

		switch(c)
		{
		case 'd':
			val  = va_arg(ap, int);

			if(val < 0)
			{
				val *= -1;
				*str++ = '-';
			}

			str = (isCol == true) ? _SprintfNumCol(val, 10, str, col, colChr, true) : _SprintfNum(val, 10, str);
			break;

		case 'x':
			str = (isCol == true) ? _SprintfHexCol((uint16_t)va_arg(ap, int), str, col, colChr, true, 'a') : _SprintfHex((uint16_t)va_arg(ap, int), str, 'a');
			break;

		case 'l':
			str = (isCol == true) ? _SprintfHexCol((uint32_t)va_arg(ap, int32_t), str, col, colChr, true, 'a') : _SprintfHex((uint32_t)va_arg(ap, int32_t), str, 'a');
			break;

		case 's':
			val2 = va_arg(ap, char*);
			str = _SprintfStr(val2, str);
			break;

		case 'c':
			val3  = va_arg(ap, int);
			*str++ = val3;
			break;

		case '\0':
		default:
			*str++ = '\0';
			return;
		}
	}
}
//---------------------------------------------------------------------------
char* _SprintfNum(int16_t val, int16_t base, char* s)
{
	int16_t c = val % base;
	val = val / base;

	if(val > 0)
	{
		s = _SprintfNum(val, base, s);
	}

	*s++ = c+'0';

	return s;
}
//---------------------------------------------------------------------------
char* _SprintfNumCol(int16_t val, int16_t base, char* s, int16_t col, char colChr, bool isTop)
{
	int16_t c = val % base;
	val = val / base;

	if(val > 0 || col > 1)
	{
		s = _SprintfNumCol(val, base, s, col-1, colChr, false);
	}

	if(c != 0 || val != 0 || isTop == true)
	{
		*s++ = c+'0';
	}
	else
	{
		*s++ = colChr;
	}

	return s;
}
//---------------------------------------------------------------------------
char* _SprintfHexCol(uint32_t val, char* s, int16_t col, char colChr, bool isTop, char hex)
{
	if(val >= 0x10 || col > 1)
	{
		s = _SprintfHexCol(val >> 4, s, col-1, colChr, false, hex);
	}

	uint16_t c = val & 0xf;

	if(c != 0 || val != 0 || isTop == true)
	{
		*s++ = (c>9) ? c-10+hex : c+'0';
	}
	else
	{
		*s++ = colChr;
	}

	return s;
}
//---------------------------------------------------------------------------
char* _SprintfHex(uint32_t val, char* s, char hex)
{
	if(val >= 0x10)
	{
		s = _SprintfHex(val >> 4, s, hex);
	}

	uint16_t c = val & 0xf;
	*s++ = (c>9) ? c-10+hex : c+'0';

	return s;
}
//---------------------------------------------------------------------------
char* _SprintfStr(char* val, char* s)
{
	while(*val != '\0')
	{
		*s++ = *val++;
	}

	return s;
}
//---------------------------------------------------------------------------
bool _IsDigit(char c)
{
	return (c >= '0') && (c <= '9') ? true : false;
}
//---------------------------------------------------------------------------
void _SystemError(const char* fmt, ...)
{
	char s[40];

	va_list ap;
	va_start(ap, fmt);
	_SprintfDo(s, fmt, ap);
	va_end(ap);


	uint16_t i, x = 0;

	for(i=0; s[i] != '\0'; i++)
	{
		OledDrawChr(x++, 0, s[i]);
	}

	OledDisplay();

	for(;;)
	{
		// EMPTY
	}
}
