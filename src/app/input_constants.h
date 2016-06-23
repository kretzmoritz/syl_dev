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

#ifndef SYLDEV_APP_INPUTCONSTANTS
#define SYLDEV_APP_INPUTCONSTANTS

#include "../common/enum.h"
#include "../framework/input_context.h"

namespace SylDev { namespace App {

BETTER_ENUM(InputAction, int,
	ClearScreen)

BETTER_ENUM(InputState, int,
	Draw)

BETTER_ENUM(InputRange, int,
	Placeholder)

typedef Framework::InputContext<InputAction, InputState, InputRange> InputContext;

} } // SylDev, App

#endif