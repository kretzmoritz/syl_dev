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

#include "../framework/raw_input_handler.h"

namespace SylDev { namespace App {

struct MenuId
{
	enum Type
	{
		File_Exit,

		Count
	};
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

	SylDev::Framework::RawInputHandler m_rawInputHandler;
};

} } // SylDev, App

#endif