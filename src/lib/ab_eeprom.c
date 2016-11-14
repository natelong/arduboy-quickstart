#include <avr/eeprom.h>
#include "ab.h"

static const uint32_t AB_EEP_CHECK = 0xDEADBEEF;
static const uint8_t* AB_EEP_START = 0;

void ab_eeprom_read(void* out, size_t size) {
    uint32_t check = eeprom_read_dword((uint32_t*)AB_EEP_START);
    if (check == AB_EEP_CHECK) {
        eeprom_read_block(out, AB_EEP_START + sizeof(uint32_t), size);
    } else {
        _Memset(out, 0, size);
    }
}

void ab_eeprom_write(const void* in, size_t size) {
    eeprom_write_dword((uint32_t*)AB_EEP_START, AB_EEP_CHECK);
    eeprom_write_block(in, (void*)(AB_EEP_START + sizeof(uint32_t)), size);
}
