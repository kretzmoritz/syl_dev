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
#include <unordered_map>

#include "raw_input_constants.h"
#include "input_types.h"

namespace SylDev { namespace Framework {

class InputContext
{
public:
	void ReadFromFile(std::string _file);
	void WriteToFile(std::string _file);

	InputAction MapButtonToAction(RawInputButton _button, InputAction _action);
	InputState MapButtonToState(RawInputButton _button, InputState _state);
	InputRange MapAxisToRange(RawInputAxis _axis, InputRange _range);

private:
	std::unordered_map<RawInputButton::_enumerated, InputAction> m_actions;
	std::unordered_map<RawInputButton::_enumerated, InputState> m_states;
	std::unordered_map<RawInputAxis::_enumerated, InputRange> m_ranges;
};

} } // SylDev, Framework

#endif