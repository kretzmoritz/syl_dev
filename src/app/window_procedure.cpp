#include "window_procedure.h"

using namespace SylDev::App;

LRESULT CALLBACK WindowProcedure::operator()(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_msg)
	{
	case SWM_INIT:
		m_Core.Init(_hWnd, reinterpret_cast<LPSTR>(_wParam));
		break;
	case SWM_UPDATE:
		m_Core.Update();
		break;
	case WM_DESTROY:
		m_Core.Release();
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		{
			switch(LOWORD(_wParam))
			{ 
			case ID_FILE_EXIT:
				m_Core.Release();
				PostQuitMessage(0);
				break;
			}
		}
		break;
	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(_hWnd, &ps);
		m_Core.Paint(ps.hdc);
		EndPaint(_hWnd, &ps);
		break;
	default:
		return DefWindowProc(_hWnd, _msg, _wParam, _lParam);
	}

	return 0;
}