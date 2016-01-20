#include "window_procedure.h"

using namespace SylDev::App;

LRESULT CALLBACK WindowProcedure::operator()(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_msg)
	{
	case SYLDEV_WM_INIT:
		m_Core.Init(_hWnd, reinterpret_cast<LPSTR>(_wParam));
		break;
	case SYLDEV_WM_UPDATE:
		m_Core.Update();
		break;
	case WM_DESTROY:
		m_Core.Release();
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		{
			switch (static_cast<MenuIds>(LOWORD(_wParam)))
			{
			case MenuIds::File_Exit:
				DestroyWindow(_hWnd);
				break;
			}
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(_hWnd, &ps);
			m_Core.Paint(ps.hdc);
			EndPaint(_hWnd, &ps);
		}
		break;
	default:
		return DefWindowProc(_hWnd, _msg, _wParam, _lParam);
	}

	return 0;
}