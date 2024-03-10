#include"pch.h"
#include "Input.h"
#include <iostream>

bool Input::keys[256] = { false };
KeyState Input::keyStates[256] = { KeyState::Up };
POINT Input::prevMousePos = { 0 };
bool Input::mouseButtons[3] = { false };
KeyState Input::mouseButtonStates[3] = { KeyState::Up };

bool Input::isKeyPressed(char key) {
    return keys[key];
}

KeyState Input::getKeyState(char key) {
    return keyStates[key];
}

POINT Input::getMouseDelta() {
    POINT currentMousePos;
    GetCursorPos(&currentMousePos);
    ScreenToClient(GetActiveWindow(), &currentMousePos);
    POINT delta = { currentMousePos.x - prevMousePos.x, currentMousePos.y - prevMousePos.y };
    prevMousePos = currentMousePos;
    return delta;
}

bool Input::isMouseButtonPressed(int button) {
    return mouseButtons[button];
}

KeyState Input::getMouseButtonState(int button) {
    return mouseButtonStates[button];
}

void Input::update(HWND hWnd) {
    // touches clavier
    for (int i = 0; i < 256; ++i) {
        bool keyDown = (GetAsyncKeyState(i) & 0x8000) != 0;
        if (keyDown && !keys[i]) {
            keyStates[i] = KeyState::Pressed;
        }
        else if (!keyDown && keys[i]) {
            keyStates[i] = KeyState::Released;
        }
        else {
            keyStates[i] = keyDown ? KeyState::Down : KeyState::Up;
        }
        keys[i] = keyDown;
    }

    // boutons de la souris
    for (int i = 0; i < 3; ++i) {
        bool buttonDown = (GetAsyncKeyState(VK_LBUTTON + i) & 0x8000) != 0;
        if (buttonDown && !mouseButtons[i]) {
            mouseButtonStates[i] = KeyState::Pressed;
        }
        else if (!buttonDown && mouseButtons[i]) {
            mouseButtonStates[i] = KeyState::Released;
        }
        else {
            mouseButtonStates[i] = buttonDown ? KeyState::Down : KeyState::Up;
        }
        mouseButtons[i] = buttonDown;
    }

    // Mise à jour de la position de la souris
    POINT currentMousePos;
    GetCursorPos(&currentMousePos);
    ScreenToClient(hWnd, &currentMousePos);
    prevMousePos = currentMousePos;
}