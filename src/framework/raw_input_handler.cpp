#include "raw_input_handler.h"

#include <Windows.h>

using namespace SylDev::Framework;

RawInputHandler::RawInputHandler()
{
	for (unsigned int i = 0; i < KeyCount; ++i)
	{
		m_pressed[i] = false;
		m_down[i] = false;
		m_toggled[i] = false;
		m_released[i] = false;
	}
}

void RawInputHandler::Refresh()
{
	// Reset pressed and released states; these should only last one frame
	for (unsigned int i = 0; i < KeyCount; ++i)
	{
		m_pressed[i] = false;
		m_released[i] = false;
	}

	// Figure out key states
	for (unsigned int i = 0; i < KeyCount; ++i)
	{
		SHORT KeyState = GetKeyState(i);

		if (HIWORD(KeyState) != 0)
		{
			if (!m_down[i])
			{
				m_pressed[i] = true;
			}

			m_down[i] = true;
		}
		else
		{
			if (m_down[i])
			{
				m_released[i] = true;
			}

			m_down[i] = false;
		}

		m_toggled[i] = LOWORD(KeyState) != 0;
	}
}

bool RawInputHandler::IsPressed(unsigned int _id)
{
	if (_id < 0 || _id >= KeyCount)
		return false;

	return m_pressed[_id];
}

bool RawInputHandler::IsDown(unsigned int _id)
{
	if (_id < 0 || _id >= KeyCount)
	return false;

	return m_down[_id];
}

bool RawInputHandler::IsToggled(unsigned int _id)
{
	if (_id < 0 || _id >= KeyCount)
		return false;

	return m_toggled[_id];
}

bool RawInputHandler::IsReleased(unsigned int _id)
{
	if (_id < 0 || _id >= KeyCount)
		return false;

	return m_released[_id];
}

SylDev::Math::Vec2i RawInputHandler::GetMousePosLocal() const
{
	POINT point;
	GetCursorPos(&point);

	ScreenToClient(GetActiveWindow(), &point);

	return SylDev::Math::Vec2i(point.x, point.y);
}

SylDev::Math::Vec2i RawInputHandler::GetMousePosScreen() const
{
	POINT point;
	GetCursorPos(&point);

	return SylDev::Math::Vec2i(point.x, point.y);
}

bool RawInputHandler::IsMouseInWindow() const
{
	RECT rect;
	GetClientRect(GetActiveWindow(), &rect);

	SylDev::Math::Vec2i MousePos = GetMousePosScreen();

	return MousePos.x >= rect.left && MousePos.x  < rect.right && MousePos.y >= rect.top && MousePos.y < rect.bottom;
}

bool RawInputHandler::HasFocus() const
{
	return GetFocus() != NULL;
}