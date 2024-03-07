#pragma once
#include "pch.h"
#include "timeapi.h"

class Time
{
public:
	Time();
	virtual ~Time();

	void Start();

	float GetElapsedTime();
	float GetTotalTime();

private:
	float fDeltaTime;
	float fTotalTime;
	DWORD dwStart;
	DWORD dwPrevious;
	DWORD dwCountTime;
	DWORD dwFPSPrevious;

};
