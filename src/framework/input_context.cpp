#include "input_context.h"

#include "../common/ini_file.h"

using namespace SylDev::Framework;

void InputContext::ReadFromFile(std::string _file)
{
	SylDev::Common::IniFile IniReader(_file);
}

void InputContext::WriteToFile(std::string _file)
{
}

InputAction InputContext::MapButtonToAction(RawInputButton::Type _button, InputAction _action)
{
	auto ii = m_Actions.emplace(_button, _action);

	if (!ii.second) // There was already an action associated with this button.
	{
		InputAction CurrentAction = ii.first->second;
		ii.first->second = _action;

		return CurrentAction;
	}

	return ii.first->second;
}

InputState InputContext::MapButtonToState(RawInputButton::Type _button, InputState _state)
{
	auto ii = m_States.emplace(_button, _state);

	if (!ii.second) // There was already a state associated with this button.
	{
		InputState CurrentState = ii.first->second;
		ii.first->second = _state;

		return CurrentState;
	}

	return ii.first->second;
}

InputRange InputContext::MapAxisToRange(RawInputAxis::Type _axis, InputRange _range)
{
	auto ii = m_Ranges.emplace(_axis, _range);

	if (!ii.second) // There was already a range associated with this axis.
	{
		InputRange CurrentRange = ii.first->second;
		ii.first->second = _range;

		return CurrentRange;
	}

	return ii.first->second;
}