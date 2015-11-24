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

#ifndef SYLDEV_FRAMEWORK_WINDOWDESC
#define SYLDEV_FRAMEWORK_WINDOWDESC

#include <Windows.h>

#include "../common/namespace.h"

BEGIN_2_NAMESPACES(SylDev, Framework)

class WindowDesc
{
public:
	WindowDesc();

	UINT style;
	HINSTANCE hInstance;
	HICON hIcon;
	HCURSOR hCursor;
	HBRUSH hbrBackground;
	HICON hIconSm;

	DWORD dwExStyle;
	LPCTSTR lpWindowName;
	DWORD dwStyle;
	int x;
	int y;
	int nWidth;
	int nHeight;

	int nCmdShow;
};

END_2_NAMESPACES

#endif