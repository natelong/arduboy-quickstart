#include "Arduino_mini.h"
#include "frame.h"
#include "sys.h"

ST_FRAME Frame;


void FrameInit(void) {
	_Memset(&Frame, 0x00, sizeof(ST_FRAME));

	FrameSetRate(60);
}

void FrameSetRate(uint8_t rate) {
	Frame.rate = rate;
	Frame.eachMillis = 1000 / rate;
}

// returns the load on the CPU as a percentage
uint16_t FrameGetCpuPercent(void) {
	return (Frame.lastDurationMs * 100) / Frame.eachMillis;
}


uint16_t FrameGetCpuPercentMax(void) {
	uint16_t per = FrameGetCpuPercent();

	if(Frame.cpuMaxPercent < per)
	{
		Frame.cpuMaxPercent = per;
	}

	return Frame.cpuMaxPercent;
}

uint8_t FrameGetCnt(void) {
	return Frame.cnt;
}

bool FrameLoop(void) {
	uint32_t now = mini_millis();

	if(Frame.isPostRender == true)
	{
		Frame.lastDurationMs = now - Frame.lastStart;
		Frame.count++;

		Frame.isPostRender = false;
	}

	if(now < Frame.nextStart)
	{
		uint8_t remaining = Frame.nextStart - now;

		if(remaining > 1)
		{
			ab_idle();
		}

		return false;
	}

	Frame.cnt++;
	Frame.nextStart    = now + Frame.eachMillis;
	Frame.lastStart    = now;
	Frame.isPostRender = true;

	return true;
}
