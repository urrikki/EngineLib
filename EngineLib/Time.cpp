#include "pch.h"
#include "Time.h"

Time::Time() {
    bIsPaused = false;
    dwPauseStart = 0;
    dwPausedTime = 0;
}

Time::~Time() {}

void Time::Start() {
    dwStart = timeGetTime();
    dwPrevious = dwStart;
    fTotalTime = 0.0f;
    fDeltaTime = 0.0f;
    dwCountTime = 0;
}

void Time::Pause() {
    if (bIsPaused == false) {
        dwPauseStart = timeGetTime();
        bIsPaused = true;
    }
}

void Time::Resume() {
    if (bIsPaused == true) {
        DWORD pauseEnd = timeGetTime();
        dwPausedTime += (pauseEnd - dwPauseStart);
        bIsPaused = false;
    }
}

bool Time::Update() {
    if (bIsPaused == true)
        return false;

    DWORD totalTimeMs = timeGetTime() - dwStart - dwPausedTime;
    DWORD dt = totalTimeMs - dwPrevious;
    if (dt < 10) {
        return false;
    }
    dwPrevious = totalTimeMs;

    if (dt > 40) {
        dt = 40;
    }
    fDeltaTime = dt / 1000.0f;
    fTotalTime += fDeltaTime;

    return true;
}

float Time::GetElapsedTime() {
    return fDeltaTime;
}

float Time::GetTotalTime() {
    return fTotalTime;
}
