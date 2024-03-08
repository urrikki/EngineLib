#include "pch.h"
#include "Time.h"

Time::Time() {}

Time::~Time() {}

void Time::Start()
{
	mStart = timeGetTime();
	mPrevious = 0;// mStart;
	mTotalTime = 0.0f;
	mDeltaTime = 0.0f;
	mCountTime = 0;
	mFPS = 0;
	mPrevFPS = 0;
}

bool Time::Update()
{
	//Current time - Previous Time
	DWORD totalTimeMs = timeGetTime() - mStart;
	DWORD dt = totalTimeMs - mPrevious;
	if (dt < 10)
	{
		return false;
	}
	mPrevious = totalTimeMs;

	UpdateFPS();


	if (dt > 40)
	{
		dt = 40;
	}

	mDeltaTime = dt / 1000.0f;
	mTotalTime += mDeltaTime;

	return true;
}

float Time::GetElapsedTime()
{
	return mDeltaTime;
}

float Time::GetTotalTime()
{
	return mTotalTime;
}

void Time::UpdateFPS()
{
	DWORD currentTime = timeGetTime();
	DWORD elapsedTime = currentTime - mFPSPrevious;
	mFPSPrevious = currentTime;

	mCountTime += elapsedTime;
	mFPS++;

	if (mCountTime >= 1000)
	{
		mPrevFPS = mFPS;
		mFPS = 0;
		mCountTime = 0;
	}
}


float Time::GetFPS()
{
	return mPrevFPS;
}