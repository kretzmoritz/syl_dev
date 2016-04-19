#include "raw_input_handler.h"

#include <Windows.h>

using namespace SylDev::Framework;

const std::map<RawInputButton, unsigned int> RawInputHandler::Mapping =
{
	{ RawInputButton::LMOUSE, VK_LBUTTON },
	{ RawInputButton::RMOUSE, VK_RBUTTON },
	{ RawInputButton::MIDDLEMOUSE, VK_MBUTTON },
	{ RawInputButton::TAB, VK_TAB },
	{ RawInputButton::ENTER, VK_RETURN },
	{ RawInputButton::ESC, VK_ESCAPE },
	{ RawInputButton::SPACE, VK_SPACE },
	{ RawInputButton::LEFT, VK_LEFT },
	{ RawInputButton::UP, VK_UP },
	{ RawInputButton::RIGHT, VK_RIGHT },
	{ RawInputButton::DOWN, VK_DOWN },
	{ RawInputButton::ZERO, 0x30 },
	{ RawInputButton::ONE, 0x31 },
	{ RawInputButton::TWO, 0x32 },
	{ RawInputButton::THREE, 0x33 },
	{ RawInputButton::FOUR, 0x34 },
	{ RawInputButton::FIVE, 0x35 },
	{ RawInputButton::SIX, 0x36 },
	{ RawInputButton::SEVEN, 0x37 },
	{ RawInputButton::EIGHT, 0x38 },
	{ RawInputButton::NINE, 0x39 },
	{ RawInputButton::A, 0x41 },
	{ RawInputButton::B, 0x42 },
	{ RawInputButton::C, 0x43 },
	{ RawInputButton::D, 0x44 },
	{ RawInputButton::E, 0x45 },
	{ RawInputButton::F, 0x46 },
	{ RawInputButton::G, 0x47 },
	{ RawInputButton::H, 0x48 },
	{ RawInputButton::I, 0x49 },
	{ RawInputButton::J, 0x4A },
	{ RawInputButton::K, 0x4B },
	{ RawInputButton::L, 0x4C },
	{ RawInputButton::M, 0x4D },
	{ RawInputButton::N, 0x4E },
	{ RawInputButton::O, 0x4F },
	{ RawInputButton::P, 0x50 },
	{ RawInputButton::Q, 0x51 },
	{ RawInputButton::R, 0x52 },
	{ RawInputButton::S, 0x53 },
	{ RawInputButton::T, 0x54 },
	{ RawInputButton::U, 0x55 },
	{ RawInputButton::V, 0x56 },
	{ RawInputButton::W, 0x57 },
	{ RawInputButton::X, 0x58 },
	{ RawInputButton::Y, 0x59 },
	{ RawInputButton::Z, 0x5A },
	{ RawInputButton::F1, 0x70 },
	{ RawInputButton::F2, 0x71 },
	{ RawInputButton::F3, 0x72 },
	{ RawInputButton::F4, 0x73 },
	{ RawInputButton::F5, 0x74 },
	{ RawInputButton::F6, 0x75 },
	{ RawInputButton::F7, 0x76 },
	{ RawInputButton::F8, 0x77 },
	{ RawInputButton::F9, 0x78 },
	{ RawInputButton::F10, 0x79 },
	{ RawInputButton::F11, 0x7A },
	{ RawInputButton::F12, 0x7B },
	{ RawInputButton::LSHIFT, 0xA0 },
	{ RawInputButton::RSHIFT, 0xA1 },
	{ RawInputButton::LCTRL, 0xA2 },
	{ RawInputButton::RCTRL, 0xA3 },
	{ RawInputButton::LALT, 0xA4 },
	{ RawInputButton::RALT, 0xA5 },
	{ RawInputButton::TILDE, VK_OEM_3 },
};

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

bool RawInputHandler::IsPressed(RawInputButton _button)
{
	unsigned int id;

	if(!GetId(_button, id))
		return false;

	if (id < 0 || id >= KeyCount)
		return false;

	return m_pressed[id];
}

bool RawInputHandler::IsDown(RawInputButton _button)
{
	unsigned int id;

	if(!GetId(_button, id))
		return false;

	if (id < 0 || id >= KeyCount)
		return false;

	return m_down[id];
}

bool RawInputHandler::IsToggled(RawInputButton _button)
{
	unsigned int id;

	if(!GetId(_button, id))
		return false;

	if (id < 0 || id >= KeyCount)
		return false;

	return m_toggled[id];
}

bool RawInputHandler::IsReleased(RawInputButton _button)
{
	unsigned int id;

	if(!GetId(_button, id))
		return false;

	if (id < 0 || id >= KeyCount)
		return false;

	return m_released[id];
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

bool RawInputHandler::GetId(RawInputButton _button, unsigned int& _id) const
{
	auto ii = Mapping.find(_button);

	if(ii == Mapping.end())
		return false;

	_id = ii->second;

	return true;
}