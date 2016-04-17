#include "core.h"

using namespace SylDev::App;

void Core::Init(HWND _hWnd, LPSTR _lpCmdLine)
{
	m_hWnd = _hWnd;

	HMENU hMenu = CreateMenu();

	HMENU hSubMenu = CreatePopupMenu();
	AppendMenu(hMenu, MF_STRING | MF_POPUP, reinterpret_cast<UINT_PTR>(hSubMenu), "&File");
	AppendMenu(hSubMenu, MF_STRING, static_cast<UINT_PTR>(MenuIds::File_Exit), "&Exit");

	SetMenu(m_hWnd, hMenu);
}

void Core::Update()
{
	m_rawInputHandler.Refresh();

	if (m_rawInputHandler.HasFocus() && (m_rawInputHandler.IsDown(VK_LBUTTON) || m_rawInputHandler.IsPressed(VK_RBUTTON)))
	{
		RedrawWindow(m_hWnd, nullptr, nullptr, RDW_INVALIDATE);
	}
}

void Core::Paint(HDC _hdc)
{
	if (m_rawInputHandler.IsPressed(VK_RBUTTON))
	{
		RECT rect;
		GetClientRect(m_hWnd, &rect);

		HBRUSH brush = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
		FillRect(_hdc, &rect, brush);

		return;
	}

	if (m_rawInputHandler.IsDown(VK_LBUTTON))
	{
		SylDev::Math::Vec2i MousePos = m_rawInputHandler.GetMousePosLocal();

		HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
		SelectObject(_hdc, brush);
		SelectObject(_hdc, GetStockObject(NULL_PEN));
		Ellipse(_hdc, MousePos.x - 25, MousePos.y - 25, MousePos.x + 25, MousePos.y + 25);

		DeleteObject(brush);
	}
}

void Core::Release()
{
}