#include "window.h"

#include <string>

#include "window_desc.h"
#include "window_procedure.h"

BEGIN_2_NAMESPACES(SylDev, Framework)

BEGIN_NAMESPACE(Impl)

unsigned int WindowBase::m_id = 0;

END_NAMESPACE

template<class T>
Window<T>::Window(WindowDesc const& _desc, LPSTR _lpCmdLine)
	: WndProc(new T()), m_thread(&Window::Create, this, _desc, _lpCmdLine)
{
}

template<class T>
Window<T>::~Window()
{
	m_thread.join();

	delete WndProc;
	WndProc = nullptr;
}

template<class T>
LRESULT CALLBACK Window<T>::StaticWndProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam)
{
	Window* wnd = nullptr;

	if (_msg == WM_NCCREATE)
	{
		wnd = reinterpret_cast<Window*>(reinterpret_cast<LPCREATESTRUCT>(_lParam)->lpCreateParams);
		SetWindowLongPtr(_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wnd));
	}
	else
	{
		wnd = reinterpret_cast<Window*>(GetWindowLongPtr(_hWnd, GWLP_USERDATA));
	}

	WindowProcedure* WndProc = nullptr;

	if (wnd)
	{
		WndProc = wnd->WndProc;
	}

	if (WndProc)
	{
		return (*WndProc)(_hWnd, _msg, _wParam, _lParam);
	}
	
	return DefWindowProc(_hWnd, _msg, _wParam, _lParam);
}

template<class T>
void Window<T>::Create(WindowDesc const&  _desc, LPSTR _lpCmdLine)
{
	std::string className = std::to_string(m_id) + ' ' + std::string(_desc.lpWindowName);
	++m_id;

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = StaticWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = _desc.hInstance;
	wc.hIcon = _desc.hIcon;
	wc.hCursor = _desc.hCursor;
	wc.hbrBackground = _desc.hbrBackground;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = className.c_str();
	wc.hIconSm = _desc.hIconSm;

	if (!RegisterClassEx(&wc))
		return;

	m_hWnd = CreateWindowEx(
		_desc.dwExStyle, className.c_str(), _desc.lpWindowName, 
		_desc.dwStyle, _desc.x, _desc.y, _desc.nWidth, _desc.nHeight, 
		nullptr, nullptr, _desc.hInstance, this);

	if (!m_hWnd)
		return;

	ShowWindow(m_hWnd, _desc.nCmdShow);
	UpdateWindow(m_hWnd);

	SendMessage(m_hWnd, SWM_INIT, reinterpret_cast<LONG_PTR>(_lpCmdLine), 0);

	MessageLoop();
}

template<class T>
void Window<T>::MessageLoop()
{
	MSG msg;
	bool quit = false;

	while (true)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				quit = true;
			}
		}

		if (quit)
		{
			break;
		}

		SendMessage(m_hWnd, SWM_UPDATE, 0, 0);
	}
}

END_2_NAMESPACES