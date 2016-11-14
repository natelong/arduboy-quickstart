#pragma once

#include "common.h"

void ab_eeprom_read(void* out, size_t size);
void ab_eeprom_write(const void* in, size_t size);