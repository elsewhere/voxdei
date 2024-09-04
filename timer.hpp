#ifndef _TIMER_HPP_
#define _TIMER_HPP_

#include <windows.h>

class Timer
{
public:
	Timer() {};
	~Timer() {};

	void init();
	void update();

	void add(float time);
	float getSeconds();
	float getMilliseconds();

private:
	bool HRT;
	float ticksPerSec;
	float startTime;
	float time, deltaTime;
	float lastTime, fps;
	float addTime;
	unsigned int frames;

};

#endif