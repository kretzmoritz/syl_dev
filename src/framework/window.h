///////////////////////////////////////
//  (             (                
//  )\ )     (    )\ )             
// (()/((    )\  (()/(    (   )    
//  /(_))\ )((_)  /(_))  ))\ /((   
// (_))(()/( _   (_))_  /((_|_))\  
// / __|)(_)) |   |   \(_)) _)((_) 
// \__ \ || | |   | |) / -_)\ V /  
// |___/\_, |_|   |___/\___| \_/   
//      |__/                       
//
// 2016 Moritz Kretz
///////////////////////////////////////

#ifndef SYLDEV_FRAMEWORK_WINDOW
#define SYLDEV_FRAMEWORK_WINDOW

#include <Windows.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <string>

#include "window_desc.h"

namespace SylDev { namespace Framework {

class WindowProcedure;

namespace Impl {

class WindowBase
{
protected:
	static std::mutex Mutex;
	static uint32_t Id;
	static std::unordered_map<std::string, std::pair<uint32_t, WindowClassDesc>> WndClasses;
};

} // Impl

struct WindowCreationResult
{
	enum Type
	{
		Unknown,
		Failed_CreateWindowClass,
		Failed_CreateWindow,
		Succeeded,

		Count
	};
};

template<class T>
class Window
	: public Impl::WindowBase
{
public:
	Window(WindowCreationResult::Type& _result, WindowClassDesc _classDesc, WindowDesc _wndDesc, LPCSTR _lpCmdLine = "");
	~Window();

private:
	static LRESULT CALLBACK StaticWndProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);
	WindowProcedure* WndProc;

	bool CreateClass(WindowClassDesc _classDesc);
	void ReleaseClass();

	void Create(WindowCreationResult::Type& _result, WindowClassDesc _classDesc, WindowDesc _wndDesc, LPCSTR _lpCmdLine);
	void Release();
	void SignalCreationFinished(WindowCreationResult::Type& _result, WindowCreationResult::Type _value);

	void MessageLoop();

	std::string m_className;
	HWND m_hWnd;
	std::thread m_thread;
	std::mutex m_conditionMutex;
	std::condition_variable m_creationFinished;
};

} } // SylDev, Framework

#include "window.tcc"

#endif