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

#include <unordered_map>
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

	InputAction MapButtonToAction(RawInputButton::Type _button, InputAction _action);
	InputState MapButtonToState(RawInputButton::Type _button, InputState _state);
	InputRange MapAxisToRange(RawInputAxis::Type _axis, InputRange _range);

private:
	static RawInputButton::Converter ButtonConverter;
	static RawInputAxis::Converter AxisConverter;

	std::unordered_map<RawInputButton::Type, InputAction> m_actions;
	std::unordered_map<RawInputButton::Type, InputState> m_states;
	std::unordered_map<RawInputAxis::Type, InputRange> m_ranges;
};

END_2_NAMESPACES

#endif