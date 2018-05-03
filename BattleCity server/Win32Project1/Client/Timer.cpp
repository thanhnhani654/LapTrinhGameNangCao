#include "Timer.h"

SYSTEMTIME Timer::getSysTime()
{
	return sysTime;
}

SYSTEMTIME Timer::getCurrentTime()
{
	GetSystemTime(&sysTime);
	simpleTime.hour = sysTime.wHour;
	simpleTime.minute = sysTime.wMinute;
	simpleTime.second = sysTime.wSecond;
	simpleTime.milisecond = sysTime.wMilliseconds;
	miliTime = simpleToMiliTime(simpleTime);
	return sysTime;
}

hmsTime Timer::getSimpleTime()
{
	return simpleTime;;
}

float Timer::getmilitime()
{
	return miliTime;
}

hmsTime Timer::miliToSimpleTime(float miliTime)
{
	hmsTime tempTime;
	tempTime.hour = miliTime / (1000 * 60 * 60);
	tempTime.minute = (miliTime - tempTime.hour * (1000 * 60 * 60)) / (1000 * 60);
	tempTime.second = (miliTime - tempTime.hour * (1000 * 60 * 60) - tempTime.minute * (1000 * 60)) / (1000);
	tempTime.milisecond = (miliTime - tempTime.hour * (1000 * 60 * 60) - tempTime.minute * (1000 * 60) - tempTime.second * 1000);
	
	return tempTime;
}

float Timer::simpleToMiliTime(hmsTime time)
{
	return time.hour * (1000 * 60 * 60) + time.minute * (1000 * 60) + time.second * 1000 + time.milisecond;
}