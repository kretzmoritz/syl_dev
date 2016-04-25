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

#ifndef SYLDEV_FRAMEWORK_RAWINPUTCONSTANTS
#define SYLDEV_FRAMEWORK_RAWINPUTCONSTANTS

#include "../common/namespace.h"
#include "../common/enum.h"

BEGIN_2_NAMESPACES(SylDev, Framework)

ENUM_WITH_STRING(RawInputButton,
	LMouse,
	RMouse,
	MiddleMouse,
	Tab,
	Enter,
	Escape,
	Space,
	Left,
	Up,
	Right,
	Down,
	Zero,
	One,
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine,
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	LShift,
	RShift,
	LCtrl,
	RCtrl,
	LAlt,
	RAlt,
	Tilde)

ENUM_WITH_STRING(RawInputAxis,
	MouseX,
	MouseY,
	MouseScroll)

END_2_NAMESPACES

#endif