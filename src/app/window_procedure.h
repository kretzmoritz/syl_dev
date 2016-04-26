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

#ifndef SYLDEV_APP_WINDOWPROCEDURE
#define SYLDEV_APP_WINDOWPROCEDURE

#include "../framework/window_procedure.h"
#include "core.h"

namespace SylDev { namespace App {

class WindowProcedure
	: public SylDev::Framework::WindowProcedure
{
public:
	virtual ~WindowProcedure() = default;

	LRESULT CALLBACK operator()(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);

private:
	Core m_Core;
};

} } // SylDev, App

#endif