#include "Time.h"

Time::Time() {}

Time::~Time() {}

void Time::Start()
{
	dwStart = timeGetTime();
	dwPrevious = 0;
	fTotalTime = 0.0f;
	fDeltaTime = 0.0f;
	dwCountTime = 0;
}

float Time::GetElapsedTime()
{
	return fDeltaTime;
}

float Time::GetTotalTime()
{
	return fTotalTime;
}
