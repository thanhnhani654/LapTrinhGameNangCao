#pragma once
#include <Windows.h>

struct hmsTime
{
	int hour;
	int minute;
	int second;
	float milisecond;
};

class Timer
{
private:
	SYSTEMTIME sysTime;
	hmsTime simpleTime;
	float miliTime;
public:
	SYSTEMTIME getSysTime();
	SYSTEMTIME getCurrentTime();
	hmsTime getSimpleTime();
	float getmilitime();
	
	hmsTime miliToSimpleTime(float miliTime);
	float simpleToMiliTime(hmsTime time);
};