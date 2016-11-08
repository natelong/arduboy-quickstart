#include <avr/power.h>
#include <avr/sleep.h>
#include "sys.h"
#include "frame.h"
#include "snd.h"
#include "key.h"
#include "oled.h"
#include "eep.h"
#include "rnd.h"

//---------------------------------------------------------------------------
void SysInit(void)
{
	power_timer2_disable();
	power_adc_disable();
	power_twi_disable();
	power_usart0_disable();
	power_usart1_disable();


	OledInit();
	FrameInit();
	SndInit();
	EepInit();
	KeyInit();
	RndInit();
}
//---------------------------------------------------------------------------
void SysIdle(void)
{
	set_sleep_mode(SLEEP_MODE_IDLE);
	sleep_mode();
}
//---------------------------------------------------------------------------
bool SysLoop(void)
{
	if(FrameLoop() == false)
	{
		return false;
	}

	KeyLoop();
	return true;
}
//---------------------------------------------------------------------------
void SysLoopEnd(void)
{
	OledDisplay();
	OledDrawCls();
}
