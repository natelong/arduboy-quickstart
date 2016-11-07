#include "Arduino_mini.h"

#include "lib/common.h"
#include "lib/sys.h"
#include "lib/oled.h"
#include "lib/key.h"

int main(void) {
    mini_init();

    SysInit();

    uint8_t x = 0;
    uint8_t y = 0;

    for (;;) {
        if(SysLoop() == false) continue;

        uint8_t trg = KeyGetTrg();
        if (trg & KEY_U) y--;
        if (trg & KEY_D) y++;
        if (trg & KEY_L) x--;
        if (trg & KEY_R) x++;

        OledDrawStr(x, y, "Arduino Mini");
        SysLoopEnd();
    }

    return 0;
}

