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

#ifndef SYLDEV_APP_WINDOWPROCEDURE
#define SYLDEV_APP_WINDOWPROCEDURE

#include "../common/namespace.h"
#include "../framework/window_procedure.h"
#include "core.h"

BEGIN_2_NAMESPACES(SylDev, App)

class WindowProcedure
	: public SylDev::Framework::WindowProcedure
{
public:
	virtual ~WindowProcedure() = default;

	LRESULT CALLBACK operator()(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);

private:
	Core m_Core;
};

END_2_NAMESPACES

#endif