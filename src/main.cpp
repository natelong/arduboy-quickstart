#include <Arduino.h>

#include "lib/common.h"
#include "lib/sys.h"
#include "lib/oled.h"

int main(void) {
    init();

#if defined(USBCON)
    USBDevice.attach();
#endif

    SysInit();

    for (;;) {
      if(SysLoop() == FALSE){
        continue;
      }

#ifdef ARD_CUSTOM
      OledDrawStr(0, 0, "Custom Arduino");
#else
      OledDrawStr(0, 0, "Standard Arduino");
#endif
      SysLoopEnd();
      if (serialEventRun) serialEventRun();
    }

    return 0;
}

