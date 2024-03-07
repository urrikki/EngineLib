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
    void Pause();
    void Resume();
    bool Update();

    float GetElapsedTime();
    float GetTotalTime();

private:
    float fDeltaTime;
    float fTotalTime;
    DWORD dwStart;
    DWORD dwPrevious;
    DWORD dwCountTime;

    bool bIsPaused;

    DWORD dwIsPaused;
    DWORD dwPauseStart;
    DWORD dwPausedTime;
};