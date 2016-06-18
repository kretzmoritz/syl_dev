#include "input_context.h"

#include "../common/ini_file.h"

namespace SylDev { namespace Framework {

template<class InputAction, class InputState, class InputRange>
void InputContext<InputAction, InputState, InputRange>::ReadFromFile(std::string _file)
{
	m_actions.clear();
	m_states.clear();
	m_ranges.clear();

	Common::IniFile iniReader(_file);

	for (size_t i = 0; i < RawInputButton::_size(); ++i)
	{
		RawInputButton button = RawInputButton::_values()[i];
		std::string actionString = iniReader.ReadString("Actions", button._to_string());

		auto action = InputAction::_from_string_nothrow(actionString.c_str());

		if (action)
		{
			MapButtonToAction(button, *action);
		}
	}

	for (size_t i = 0; i < RawInputButton::_size(); ++i)
	{
		RawInputButton button = RawInputButton::_values()[i];
		std::string stateString = iniReader.ReadString("States", button._to_string());

		auto state = InputState::_from_string_nothrow(stateString.c_str());

		if (state)
		{
			MapButtonToState(button, *state);
		}
	}

	for (size_t i = 0; i < RawInputAxis::_size(); ++i)
	{
		RawInputAxis axis = RawInputAxis::_values()[i];
		std::string rangeString = iniReader.ReadString("Ranges", axis._to_string());

		auto range = InputRange::_from_string_nothrow(rangeString.c_str());

		if (range)
		{
			MapAxisToRange(axis, *range);
		}
	}
}

template<class InputAction, class InputState, class InputRange>
void InputContext<InputAction, InputState, InputRange>::WriteToFile(std::string _file)
{
	Common::IniFile iniWriter(_file);

	for (size_t i = 0; i < RawInputButton::_size(); ++i)
	{
		RawInputButton button = RawInputButton::_values()[i];
		std::string actionString = "";

		auto ii = m_actions.find(button);

		if (ii != m_actions.end())
		{
			actionString = ii->second._to_string();
		}

		iniWriter.WriteString("Actions", button._to_string(), actionString);
	}

	for (size_t i = 0; i < RawInputButton::_size(); ++i)
	{
		RawInputButton button = RawInputButton::_values()[i];
		std::string stateString = "";

		auto ii = m_states.find(button);

		if (ii != m_states.end())
		{
			stateString = ii->second._to_string();
		}

		iniWriter.WriteString("States", button._to_string(), stateString);
	}

	for (size_t i = 0; i < RawInputAxis::_size(); ++i)
	{
		RawInputAxis axis = RawInputAxis::_values()[i];
		std::string rangeString = "";

		auto ii = m_ranges.find(axis);

		if (ii != m_ranges.end())
		{
			rangeString = ii->second._to_string();
		}

		iniWriter.WriteString("Ranges", axis._to_string(), rangeString);
	}
}

template<class InputAction, class InputState, class InputRange>
InputAction InputContext<InputAction, InputState, InputRange>::MapButtonToAction(RawInputButton _button, InputAction _action)
{
	auto ii = m_actions.emplace(_button, _action);

	if (!ii.second) // There was already an action associated with this button.
	{
		InputAction currentAction = ii.first->second;
		ii.first->second = _action;

		return currentAction;
	}

	return ii.first->second;
}

template<class InputAction, class InputState, class InputRange>
InputState InputContext<InputAction, InputState, InputRange>::MapButtonToState(RawInputButton _button, InputState _state)
{
	auto ii = m_states.emplace(_button, _state);

	if (!ii.second) // There was already a state associated with this button.
	{
		InputState currentState = ii.first->second;
		ii.first->second = _state;

		return currentState;
	}

	return ii.first->second;
}

template<class InputAction, class InputState, class InputRange>
InputRange InputContext<InputAction, InputState, InputRange>::MapAxisToRange(RawInputAxis _axis, InputRange _range)
{
	auto ii = m_ranges.emplace(_axis, _range);

	if (!ii.second) // There was already a range associated with this axis.
	{
		InputRange currentRange = ii.first->second;
		ii.first->second = _range;

		return currentRange;
	}

	return ii.first->second;
}

} } // SylDev, Framework