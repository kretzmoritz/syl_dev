#include "core.h"

namespace SylDev { namespace App {

Core::Core(Framework::RawInputHandler const& _rawInputHandler)
	: m_rawInputHandler(_rawInputHandler),
	m_inputSystem(_rawInputHandler)
{
}

void Core::Init(HWND _hWnd, LPSTR _lpCmdLine)
{
	m_hWnd = _hWnd;

	HMENU hMenu = CreateMenu();

	HMENU hSubMenu = CreatePopupMenu();
	AppendMenu(hMenu, MF_STRING | MF_POPUP, reinterpret_cast<UINT_PTR>(hSubMenu), L"&File");
	AppendMenu(hSubMenu, MF_STRING, MenuId::File_Exit, L"&Exit");

	SetMenu(m_hWnd, hMenu);

	if (!m_inputContext.ReadFromFile("input/default.ini"))
	{
		m_inputContext.MapButtonToState(Framework::RawInputButton::LMouse, InputState::Draw);
		m_inputContext.MapButtonToAction(Framework::RawInputButton::RMouse, InputAction::ClearScreen);
	}

	m_inputSystem.AddContext("default", m_inputContext);
	m_inputSystem.ActivateContext("default");
}

void Core::Update()
{
	m_inputSystem.Update();

	if (m_inputSystem.Check(InputState::Draw) || m_inputSystem.Check(InputAction::ClearScreen))
	{
		RedrawWindow(m_hWnd, nullptr, nullptr, RDW_INVALIDATE);
	}
}

void Core::Paint(HDC _hdc)
{
	if (m_inputSystem.Check(InputAction::ClearScreen))
	{
		RECT rect;
		GetClientRect(m_hWnd, &rect);

		HBRUSH brush = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
		FillRect(_hdc, &rect, brush);

		return;
	}

	if (m_inputSystem.Check(InputState::Draw))
	{
		Math::Vec2i MousePos = m_rawInputHandler.GetMousePosLocal();

		HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
		SelectObject(_hdc, brush);
		SelectObject(_hdc, GetStockObject(NULL_PEN));
		Ellipse(_hdc, MousePos.x - 25, MousePos.y - 25, MousePos.x + 25, MousePos.y + 25);

		DeleteObject(brush);
	}
}

void Core::Release()
{
	m_inputContext.WriteToFile("input/default.ini");
}

} } // SylDev, App