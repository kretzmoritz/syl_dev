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

#ifndef SYLDEV_APP_CORE
#define SYLDEV_APP_CORE

#include <Windows.h>

#include "../common/namespace.h"

BEGIN_2_NAMESPACES(SylDev, App)

enum class MenuIds
{
	File_Exit = 0,
};

class Core
{
public:
	void Init(HWND _hWnd, LPSTR _lpCmdLine);
	void Update();
	void Paint(HDC _hdc);
	void Release();

private:
	HWND m_hWnd;

	bool m_LButtonPressed;
	bool m_RButtonPressed;
};

END_2_NAMESPACES

#endif