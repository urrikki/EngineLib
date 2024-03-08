#pragma once
#pragma comment(lib, "Winmm.lib")

#include "pch.h"
#include "timeapi.h"

class Time {
public:
    Time();
    ~Time();

    void Start();
    void Pause();
    void Resume();
    void Update();
    float GetElapsedTime();
    float GetTotalTime();
    float GetFPS();

private:
    DWORD dwStart;
    DWORD dwPrevious;
    DWORD dwPauseStart;
    DWORD dwPausedTime;
    float fTotalTime;
    float fDeltaTime;
    bool bIsPaused;
    int iFrameCount;
    float fFPSTime;
    float fFPS;
};