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

#ifndef SYLDEV_FRAMEWORK_INPUTCONTEXT
#define SYLDEV_FRAMEWORK_INPUTCONTEXT

#include <string>

#include "../common/namespace.h"
#include "raw_input_constants.h"
#include "input_types.h"

BEGIN_2_NAMESPACES(SylDev, Framework)

class InputContext
{
public:
	void ReadFromFile(std::string _file);
	void WriteToFile(std::string _file);

	unsigned int MapButtonToAction(RawInputButton::Type _button, InputAction _action);
	unsigned int MapButtonToState(RawInputButton::Type _button, InputState _state);
	unsigned int MapAxisToRange(RawInputAxis::Type _axis, InputRange _range);
};

END_2_NAMESPACES

#endif