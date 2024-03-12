#pragma once

#include"pch.h"
#include "Framework.h"

enum class KeyState {
    Up,
    Down,
    Pressed,
    Released
};

class Input {
private:
    static bool keys[256];
    static KeyState keyStates[256];
    static POINT prevMousePos;
    static bool mouseButtons[3];
    static KeyState mouseButtonStates[3];

public:
    static bool isKeyPressed(char key);
    static KeyState getKeyState(char key);
    static POINT getMouseDelta();
    static bool isMouseButtonPressed(int button); // 0 = gauche, 1 = droit, 2 = milieu
    static KeyState getMouseButtonState(int button);
    static void update(HWND hWnd);
};
