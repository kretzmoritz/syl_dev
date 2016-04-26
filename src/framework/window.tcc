#include "window.h"

#include "window_procedure.h"

namespace SylDev { namespace Framework {

namespace Impl {

std::mutex WindowBase::Mutex;
uint32_t WindowBase::Id = 0;
std::unordered_map<std::string, std::pair<uint32_t, WindowClassDesc>> WindowBase::WndClasses;

} // Impl

template<class T>
Window<T>::Window(WindowCreationResult::Type& _result, WindowClassDesc _classDesc, WindowDesc _wndDesc, LPSTR _lpCmdLine)
	: WndProc(new T())
{
	std::unique_lock<std::mutex> lock(m_conditionMutex);

	WindowCreationResult::Type result = WindowCreationResult::Unknown;
	m_thread = std::move(std::thread(&Window::Create, this, std::ref(result), _classDesc, _wndDesc, _lpCmdLine));

	do
	{
		m_creationFinished.wait(lock);
	} while (result == WindowCreationResult::Unknown);

	_result = result;
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
	std::lock_guard<std::mutex> lock(Mutex);

	std::string className;

	for (auto ii = WndClasses.begin(); ii != WndClasses.end(); ++ii)
	{
		WindowClassDesc classDesc = ii->second.second;

		if (_classDesc == classDesc)
		{
			uint32_t& count = ii->second.first;
			++count;

			className = ii->first;

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
			return false;
		}

		WndClasses.emplace(className, std::make_pair(1, _classDesc));

		++Id;
	}

	m_className = className;

	return true;
}

template<class T>
void Window<T>::ReleaseClass()
{
	std::lock_guard<std::mutex> lock(Mutex);

	auto ii = WndClasses.find(m_className);

	if (ii == WndClasses.end())
	{
		return;
	}

	uint32_t& count = ii->second.first;
	--count;

	if (count == 0)
	{
		HINSTANCE hInstance = ii->second.second.hInstance;

		UnregisterClass(m_className.c_str(), hInstance);
		WndClasses.erase(ii);
	}

	m_className.clear();
}

template<class T>
void Window<T>::Create(WindowCreationResult::Type& _result, WindowClassDesc _classDesc, WindowDesc _wndDesc, LPSTR _lpCmdLine)
{
	if (!CreateClass(_classDesc))
	{
		Release();
		SignalCreationFinished(_result, WindowCreationResult::Failed_CreateWindowClass);

		return;
	}

	m_hWnd = CreateWindowEx(
		_wndDesc.dwExStyle, m_className.c_str(), _wndDesc.lpWindowName, 
		_wndDesc.dwStyle, _wndDesc.x, _wndDesc.y, _wndDesc.nWidth, _wndDesc.nHeight, 
		nullptr, nullptr, _classDesc.hInstance, this);

	if (!m_hWnd)
	{
		Release();
		SignalCreationFinished(_result, WindowCreationResult::Failed_CreateWindow);

		return;
	}

	ShowWindow(m_hWnd, _wndDesc.nCmdShow);
	UpdateWindow(m_hWnd);

	SignalCreationFinished(_result, WindowCreationResult::Succeeded);

	SendMessage(m_hWnd, SYLDEV_WM_INIT, reinterpret_cast<LONG_PTR>(_lpCmdLine), 0);

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
void Window<T>::SignalCreationFinished(WindowCreationResult::Type& _result, WindowCreationResult::Type _value)
{
	std::unique_lock<std::mutex> lock(m_conditionMutex);

	_result = _value;
	m_creationFinished.notify_all();
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

		SendMessage(m_hWnd, SYLDEV_WM_UPDATE, 0, 0);
	}
}

} } // SylDev, Framework