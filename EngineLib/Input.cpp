#include"pch.h"
#include "Input.h"
#include <iostream>


Input::Input()
{
}

bool Input::isMouseDown(const  MouseButton& button)
{
	if (button == MouseButton::Left)
	{
		auto state = m_keys_state_res[VK_LBUTTON];
		return (state == 0);

	}
	else if (button == MouseButton::Middle)
	{
		auto state = m_keys_state_res[VK_MBUTTON];
		return (state == 0);
	}
	else if (button == MouseButton::Right)
	{
		auto state = m_keys_state_res[VK_RBUTTON];
		return (state == 0);
	}

	return false;
}

bool Input::isMouseUp(const  MouseButton& button)
{
	if (button == MouseButton::Left)
	{
		auto state = m_keys_state_res[VK_LBUTTON];
		return  (state == 1);

	}
	else if (button == MouseButton::Middle)
	{
		auto state = m_keys_state_res[VK_MBUTTON];
		return  (state == 1);
	}
	else if (button == MouseButton::Right)
	{
		auto state = m_keys_state_res[VK_RBUTTON];
		return (state == 1);
	}
	return false;
}

float Input::getMouseXAxis()
{
	return m_deltaMouse.x;
}

float Input::getMouseYAxis()
{
	return m_deltaMouse.y;
}

void Input::enablePlayMode(bool enable)
{
	m_playEnable = enable;
	ShowCursor(!enable);
}

void Input::setScreenArea(const  Rect& area)
{
	m_screenArea = area;
}

void Input::update()
{
	POINT current_mouse_pos = {};
	::GetCursorPos(&current_mouse_pos);

	if (current_mouse_pos.x != (i32)m_old_mouse_pos.x || current_mouse_pos.y != (i32)m_old_mouse_pos.y)
	{
		m_deltaMouse = Vec2((float)current_mouse_pos.x - (float)m_old_mouse_pos.x, (float)current_mouse_pos.y - (float)m_old_mouse_pos.y);
	}
	else
	{
		m_deltaMouse = Vec2(0, 0);
	}


	if (!m_playEnable)
	{
		m_old_mouse_pos = Vec2((float)current_mouse_pos.x, (float)current_mouse_pos.y);
	}
	else
	{
		Vec2 center_screen = Vec2(m_screenArea.left + (float)m_screenArea.width / 2.0f, m_screenArea.top + (float)m_screenArea.height / 2.0f);
		::SetCursorPos((i32)center_screen.x, (i32)center_screen.y);
		m_old_mouse_pos = center_screen;
	}


	for (unsigned int i = 0; i < 256; i++)
	{
		m_keys_state[i] = ::GetAsyncKeyState(i);


		if (m_keys_state[i] & 0x8001)
		{
			m_keys_state_res[i] = 0;
		}
		else // KEY IS UP
		{
			if (m_keys_state[i] != m_old_keys_state[i])
			{
				m_keys_state_res[i] = 1;
			}
			else
			{
				m_keys_state_res[i] = 2;
			}
		}
	}

	::memcpy(m_old_keys_state, m_keys_state, sizeof(short) * 256);
}