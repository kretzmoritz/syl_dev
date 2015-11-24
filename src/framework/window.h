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

#include "../common/namespace.h"

BEGIN_2_NAMESPACES(SylDev, Framework)

class WindowDesc;
class WindowProcedure;

BEGIN_NAMESPACE(Impl)

class WindowBase
{
protected:
	WindowBase() = default;
	WindowBase(WindowBase const&) = default;

	static unsigned int m_id;
};

END_NAMESPACE

template<class T>
class Window
	: public Impl::WindowBase
{
public:
	Window(WindowDesc const& _desc, LPSTR _lpCmdLine);
	~Window();

private:
	static LRESULT CALLBACK StaticWndProc(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);
	WindowProcedure* WndProc;

	void Create(WindowDesc const& _desc, LPSTR _lpCmdLine);
	void MessageLoop();

	HWND m_hWnd;
	std::thread m_thread;
};

END_2_NAMESPACES

#include "window.tcc"

#endif