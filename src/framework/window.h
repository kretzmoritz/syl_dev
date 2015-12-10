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
// 2015 Moritz Kretz
///////////////////////////////////////

#ifndef SYLDEV_FRAMEWORK_WINDOW
#define SYLDEV_FRAMEWORK_WINDOW

#include <Windows.h>
#include <thread>
#include <mutex>
#include <map>
#include <string>

#include "../common/namespace.h"
#include "window_desc.h"

BEGIN_2_NAMESPACES(SylDev, Framework)

class WindowProcedure;

BEGIN_NAMESPACE(Impl)

class WindowBase
{
protected:
	static std::mutex Mutex;
	static unsigned int Id;
	static std::map<std::string, std::pair<unsigned int, WindowClassDesc>> WndClasses;
};

END_NAMESPACE

template<class T>
class Window
	: public Impl::WindowBase
{
public:
	Window(WindowClassDesc _classDesc, WindowDesc _wndDesc, LPSTR _lpCmdLine = "");
	~Window();

private:
	static LRESULT CALLBACK StaticWndProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);
	WindowProcedure* WndProc;

	bool CreateClass(WindowClassDesc _classDesc);
	void ReleaseClass();

	void Create(WindowClassDesc _classDesc, WindowDesc _wndDesc, LPSTR _lpCmdLine);
	void Release();

	void MessageLoop();

	std::string m_className;
	HWND m_hWnd;
	std::thread m_thread;
};

END_2_NAMESPACES

#include "window.tcc"

#endif