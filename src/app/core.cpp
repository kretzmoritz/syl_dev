#include "core.h"

#include <iostream>

using namespace SylDev::App;

void Core::Init(HWND _hWnd, LPSTR _lpCmdLine)
{
	m_hWnd = _hWnd;

	m_LButtonPressed = false;
	m_RButtonPressed = false;

	std::cout << _lpCmdLine << std::endl;

	HMENU hMenu = CreateMenu();

	HMENU hSubMenu = CreatePopupMenu();
	AppendMenu(hMenu, MF_STRING | MF_POPUP, reinterpret_cast<UINT_PTR>(hSubMenu), "&File");
	AppendMenu(hSubMenu, MF_STRING, ID_FILE_EXIT, "&Exit");

	SetMenu(m_hWnd, hMenu);
}

void Core::Update()
{
	m_LButtonPressed = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
	m_RButtonPressed = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;

	if(m_LButtonPressed || m_RButtonPressed)
	{
		RedrawWindow(m_hWnd, nullptr, nullptr, RDW_INVALIDATE);
	}
}

void Core::Paint(HDC hdc)
{
	if(m_RButtonPressed)
	{
		RECT rect;
		GetWindowRect(m_hWnd, &rect);

		HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
		FillRect(hdc, &rect, brush);

		DeleteObject(brush);

		return;
	}

	if(m_LButtonPressed)
	{
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(m_hWnd, &point);

		HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
		SelectObject(hdc, brush);
		Ellipse(hdc, point.x - 25, point.y - 25, point.x + 25, point.y + 25);

		DeleteObject(brush);
	}
}

void Core::Release()
{
}