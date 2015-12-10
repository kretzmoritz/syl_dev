#include "window.h"

#include "window_procedure.h"

BEGIN_2_NAMESPACES(SylDev, Framework)

BEGIN_NAMESPACE(Impl)

std::mutex WindowBase::Mutex;
unsigned int WindowBase::Id = 0;
std::map<std::string, std::pair<unsigned int, WindowClassDesc>> WindowBase::WndClasses;

END_NAMESPACE

template<class T>
Window<T>::Window(WindowClassDesc _classDesc, WindowDesc _wndDesc, LPSTR _lpCmdLine)
	: WndProc(new T()), m_thread(&Window::Create, this, _classDesc, _wndDesc, _lpCmdLine)
{
}

template<class T>
Window<T>::~Window()
{
	m_thread.join();

	Release();
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
bool Window<T>::CreateClass(WindowClassDesc _classDesc)
{
	bool result = true;

	Mutex.lock();

	do
	{
		std::string className;

		for (auto it = WndClasses.begin(); it != WndClasses.end(); ++it)
		{
			WindowClassDesc classDesc = it->second.second;

			if (_classDesc == classDesc)
			{
				unsigned int& count = it->second.first;
				++count;

				className = it->first;

				break;
			}
		}

		if (className.empty())
		{
			className = "SylWindowClass" + std::to_string(Id);

			WNDCLASSEX wc;
			wc.cbSize = sizeof(WNDCLASSEX);
			wc.style = _classDesc.style;
			wc.lpfnWndProc = StaticWndProc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = _classDesc.hInstance;
			wc.hIcon = _classDesc.hIcon;
			wc.hCursor = _classDesc.hCursor;
			wc.hbrBackground = _classDesc.hbrBackground;
			wc.lpszMenuName = nullptr;
			wc.lpszClassName = className.c_str();
			wc.hIconSm = _classDesc.hIconSm;

			if (!RegisterClassEx(&wc))
			{
				result = false;
				break;
			}

			WndClasses.emplace(className, std::make_pair(1, _classDesc));

			++Id;
		}

		m_className = className;
	} while (false);

	Mutex.unlock();

	return result;
}

template<class T>
void Window<T>::ReleaseClass()
{
	Mutex.lock();

	do
	{
		auto it = WndClasses.find(m_className);

		if (it == WndClasses.end())
		{
			break;
		}

		unsigned int& count = it->second.first;
		--count;

		if (count == 0)
		{
			HINSTANCE hInstance = it->second.second.hInstance;

			UnregisterClass(m_className.c_str(), hInstance);
			WndClasses.erase(it);
		}

		m_className.clear();
	} while (false);

	Mutex.unlock();
}

template<class T>
void Window<T>::Create(WindowClassDesc _classDesc, WindowDesc _wndDesc, LPSTR _lpCmdLine)
{
	if (!CreateClass(_classDesc))
	{
		Release();
		return;
	}

	m_hWnd = CreateWindowEx(
		_wndDesc.dwExStyle, m_className.c_str(), _wndDesc.lpWindowName, 
		_wndDesc.dwStyle, _wndDesc.x, _wndDesc.y, _wndDesc.nWidth, _wndDesc.nHeight, 
		nullptr, nullptr, _classDesc.hInstance, this);

	if (!m_hWnd)
	{
		Release();
		return;
	}

	ShowWindow(m_hWnd, _wndDesc.nCmdShow);
	UpdateWindow(m_hWnd);

	SendMessage(m_hWnd, SWM_INIT, reinterpret_cast<LONG_PTR>(_lpCmdLine), 0);

	MessageLoop();
	Release();
}

template<class T>
void Window<T>::Release()
{
	delete WndProc;
	WndProc = nullptr;

	ReleaseClass();
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