#pragma once
#pragma comment(lib, "Winmm.lib")

#include "pch.h"
#include "timeapi.h"

class Time
{
public:
	Time();
	virtual ~Time();

	void Start();
	bool Update();

	float GetElapsedTime();
	float GetTotalTime();
	void UpdateFPS();
	float GetFPS();

private:
	float mDeltaTime;
	float mTotalTime;
	DWORD mStart;
	DWORD mPrevious;
	DWORD mCountTime;
	DWORD mFPSPrevious;
	int mFPS;
	int mPrevFPS;

};