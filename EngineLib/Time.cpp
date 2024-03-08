#include "pch.h"
#include "Time.h"

Time::Time() {
    bIsPaused = false;
    dwPauseStart = 0;
    dwPausedTime = 0;
    iFrameCount = 0;
    fFPS = 0.0f;
}

Time::~Time() {}

void Time::Start() {
    dwStart = timeGetTime();
    dwPrevious = dwStart;
    fTotalTime = 0.0f;
    fDeltaTime = 0.0f;
    iFrameCount = 0;
    fFPS = 0.0f;
}

void Time::Pause() {
    if (!bIsPaused) {
        dwPauseStart = timeGetTime();
        bIsPaused = true;
    }
}

void Time::Resume() {
    if (bIsPaused) {
        DWORD pauseEnd = timeGetTime();
        dwPausedTime += (pauseEnd - dwPauseStart);
        bIsPaused = false;
    }
}

void Time::Update() {
    if (bIsPaused)
        return;

    DWORD totalTimeMs = timeGetTime() - dwStart - dwPausedTime;
    DWORD dt = totalTimeMs - dwPrevious;
    dwPrevious = totalTimeMs;

    if (dt < 10) {
        return;
    }

    fDeltaTime = dt / 1000.0f;
    fTotalTime += fDeltaTime;

    iFrameCount++;
    if (fTotalTime >= 1.0f) {
        fFPS = iFrameCount;
        iFrameCount = 0;
        fTotalTime -= 1.0f; 
    }
}

float Time::GetElapsedTime() {
    return fDeltaTime;
}

float Time::GetTotalTime() {
    return fFPS;
}

float Time::GetFPS() {
    return fFPS;
}