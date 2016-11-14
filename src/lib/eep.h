#pragma once

#include "common.h"

void ab_eep_read(void* out, size_t size);
void ab_eep_write(const void* in, size_t size);