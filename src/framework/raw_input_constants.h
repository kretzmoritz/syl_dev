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

BEGIN_2_NAMESPACES(SylDev, Framework)

enum class RawInputButton
{
	LMOUSE,
	RMOUSE,
	MIDDLEMOUSE,
	TAB,
	ENTER,
	ESC,
	SPACE,
	LEFT,
	UP,
	RIGHT,
	DOWN,
	ZERO,
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
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
	LSHIFT,
	RSHIFT,
	LCTRL,
	RCTRL,
	LALT,
	RALT,
	TILDE,

	NUM_OF
};

enum class RawInputAxis
{
	MOUSE_X,
	MOUSE_Y,
	MOUSE_SCROLL
};

END_2_NAMESPACES

#endif