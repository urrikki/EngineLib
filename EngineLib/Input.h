#pragma once

#include"pch.h"
#include "Framework.h"
#include "Math.h"

enum class  MouseButton
{
	Left = 0,
	Middle,
	Right
};

class Input {
public:
	Input();

	virtual bool isMouseDown(const  MouseButton& button);
	virtual bool isMouseUp(const  MouseButton& button);

	virtual float getMouseXAxis();
	virtual float getMouseYAxis();

	virtual void enablePlayMode(bool enable);

	void setScreenArea(const  Rect& area);
	void update();

private:
	short m_keys_state[256] = {};
	short m_old_keys_state[256] = {};
	short m_keys_state_res[256] = {};

	bool m_playEnable = false;
	Vec2 m_old_mouse_pos;
	bool m_first_time = true;
	Rect m_screenArea;
	Vec2 m_deltaMouse;
	int states_index = 0;
};