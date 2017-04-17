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

#include "../framework/input_context.h"

namespace SylDev { namespace App {

BETTER_ENUM(InputAction, int32_t,
	ClearScreen)

BETTER_ENUM(InputState, int32_t,
	Draw)

BETTER_ENUM(InputRange, int32_t,
	Placeholder)

typedef Framework::InputContext<InputAction, InputState, InputRange> InputContext;

} } // SylDev, App

#endif