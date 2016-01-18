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

#ifndef SYLDEV_FRAMEWORK_WINDOWPROCEDURE
#define SYLDEV_FRAMEWORK_WINDOWPROCEDURE

#include <Windows.h>

#include "../common/namespace.h"

BEGIN_2_NAMESPACES(SylDev, Framework)

#define SWM_INIT (WM_USER + 0)
#define SWM_UPDATE (WM_USER + 1)

class WindowProcedure
{
public:
	virtual ~WindowProcedure() = default;

	virtual LRESULT CALLBACK operator()(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam) = 0;
};

END_2_NAMESPACES

#endif