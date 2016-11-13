#pragma once

#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

//---------------------------------------------------------------------------
#define ASSERT(x) if(!(x)) _SystemError("%s(%d):"#x"\n", __FILE__, __LINE__)

//---------------------------------------------------------------------------
void  _Memset(void* s, uint8_t c, int16_t size);
char* _Sprintf(char* buf, const char* fmt, ...);
void  _SprintfDo(char* str, const char* fmt, va_list ap);
char* _SprintfNum(int16_t val, int16_t base, char* s);
char* _SprintfNumCol(int16_t val, int16_t base, char* s, int16_t col, char colChr, bool isTop);
char* _SprintfHex(uint32_t val, char* s, char hex);
char* _SprintfHexCol(uint32_t val, char* s, int16_t col, char colChr, bool isTop, char hex);
char* _SprintfStr(char* val, char* s);
bool  _IsDigit(char c);

void  _SystemError(const char* f, ...);
