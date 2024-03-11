#pragma once

#include "pch.H"
#include "Input.h"
#include <Windows.h>
#include <string>

class Button {
public:
    Button(RECT bounds, std::string text);
    ~Button();

    bool IsClicked() const;
    void Render() const;

    std::string GetText() const;

private:
    RECT mBounds;
    std::string mText;
};

