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

#ifndef SYLDEV_FRAMEWORK_WINDOWDESC
#define SYLDEV_FRAMEWORK_WINDOWDESC

#include <Windows.h>

namespace SylDev { namespace Framework {

class WindowClassDesc
{
public:
	WindowClassDesc();

	bool operator==(WindowClassDesc const& _rhs);

	UINT style;
	HINSTANCE hInstance;
	HICON hIcon;
	HCURSOR hCursor;
	HBRUSH hbrBackground;
	HICON hIconSm;
};

class WindowDesc
{
public:
	WindowDesc();

	DWORD dwExStyle;
	LPCSTR lpWindowName;
	DWORD dwStyle;
	int x;
	int y;
	int nWidth;
	int nHeight;

	int nCmdShow;
};

} } // SylDev, Framework

#endif