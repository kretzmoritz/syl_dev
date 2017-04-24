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
#include <map>

#include "raw_input_constants.h"

namespace SylDev { namespace Framework {

template<class InputAction, class InputState, class InputRange>
class InputContext
{
public:
	bool ReadFromFile(std::string _file);
	void WriteToFile(std::string _file);

	std::map<RawInputButton, InputAction> const& GetActionMapping() const;
	std::map<RawInputButton, InputState> const& GetStateMapping() const;
	std::map<RawInputAxis, InputRange> const& GetRangeMapping() const;

	InputAction MapButtonToAction(RawInputButton _button, InputAction _action);
	InputState MapButtonToState(RawInputButton _button, InputState _state);
	InputRange MapAxisToRange(RawInputAxis _axis, InputRange _range);

	typedef InputAction ActionType;
	typedef InputState StateType;
	typedef InputRange RangeType;

private:
	std::map<RawInputButton, InputAction> m_actions;
	std::map<RawInputButton, InputState> m_states;
	std::map<RawInputAxis, InputRange> m_ranges;
};

} } // SylDev, Framework

#include "input_context.tcc"

#endif