#include "input_context.h"

#include "../common/ini_file.h"

using namespace SylDev::Framework;
using namespace SylDev::Common;

void InputContext::ReadFromFile(std::string _file)
{
	m_actions.clear();
	m_states.clear();
	m_ranges.clear();

	IniFile iniReader(_file);

	for (size_t i = 0; i < RawInputButton::_size(); ++i)
	{
		RawInputButton button = RawInputButton::_values()[i];
		std::string actionString = iniReader.ReadString("Actions", button._to_string());

		if (InputAction::CheckAction(actionString))
		{
			InputAction action = InputAction::GetAction(actionString);

			MapButtonToAction(button, action);
		}
	}

	for (size_t i = 0; i < RawInputButton::_size(); ++i)
	{
		RawInputButton button = RawInputButton::_values()[i];
		std::string stateString = iniReader.ReadString("States", button._to_string());

		if (InputState::CheckState(stateString))
		{
			InputState state = InputState::GetState(stateString);

			MapButtonToState(button, state);
		}
	}

	for (size_t i = 0; i < RawInputAxis::_size(); ++i)
	{
		RawInputAxis axis = RawInputAxis::_values()[i];
		std::string rangeString = iniReader.ReadString("Ranges", axis._to_string());

		if (InputRange::CheckRange(rangeString))
		{
			InputRange range = InputRange::GetRange(rangeString);

			MapAxisToRange(axis, range);
		}
	}
}

void InputContext::WriteToFile(std::string _file)
{
	IniFile iniWriter(_file);

	for (size_t i = 0; i < RawInputButton::_size(); ++i)
	{
		RawInputButton button = RawInputButton::_values()[i];
		std::string actionString = "";

		auto ii = m_actions.find(button);

		if (ii != m_actions.end())
		{
			actionString = ii->second.ToString();
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
			stateString = ii->second.ToString();
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
			rangeString = ii->second.ToString();
		}

		iniWriter.WriteString("Ranges", axis._to_string(), rangeString);
	}
}

InputAction InputContext::MapButtonToAction(RawInputButton _button, InputAction _action)
{
	if (_action.ToString().empty())
		return _action;

	auto ii = m_actions.emplace(_button, _action);

	if (!ii.second) // There was already an action associated with this button.
	{
		InputAction currentAction = ii.first->second;
		ii.first->second = _action;

		return currentAction;
	}

	return ii.first->second;
}

InputState InputContext::MapButtonToState(RawInputButton _button, InputState _state)
{
	if (_state.ToString().empty())
		return _state;

	auto ii = m_states.emplace(_button, _state);

	if (!ii.second) // There was already a state associated with this button.
	{
		InputState currentState = ii.first->second;
		ii.first->second = _state;

		return currentState;
	}

	return ii.first->second;
}

InputRange InputContext::MapAxisToRange(RawInputAxis _axis, InputRange _range)
{
	if (_range.ToString().empty())
		return _range;

	auto ii = m_ranges.emplace(_axis, _range);

	if (!ii.second) // There was already a range associated with this axis.
	{
		InputRange currentRange = ii.first->second;
		ii.first->second = _range;

		return currentRange;
	}

	return ii.first->second;
}