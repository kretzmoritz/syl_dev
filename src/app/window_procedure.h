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
#include "../framework/raw_input_handler.h"
#include "core.h"

namespace SylDev { namespace App {

class WindowProcedure
	: public Framework::WindowProcedure
{
public:
	WindowProcedure();
	virtual ~WindowProcedure() = default;

	LRESULT CALLBACK operator()(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);

private:
	Framework::RawInputHandler m_rawInputHandler;
	Core m_Core;
};

} } // SylDev, App

#endif