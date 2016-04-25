#include "input_context.h"

#include "../common/ini_file.h"

using namespace SylDev::Framework;
using namespace SylDev::Common;

RawInputButton::Converter InputContext::ButtonConverter;
RawInputAxis::Converter InputContext::AxisConverter;

void InputContext::ReadFromFile(std::string _file)
{
	IniFile iniReader(_file);

	for (uint32_t i = 0; i < RawInputButton::Count; ++i)
	{
		RawInputButton::Type button = static_cast<RawInputButton::Type>(i);
		std::string buttonString = ButtonConverter.ToString(button);
		
		std::string actionString = iniReader.ReadString("Actions", buttonString);

		if (InputAction::CheckAction(actionString))
		{
			InputAction action = InputAction::GetAction(actionString);

			MapButtonToAction(button, action);
		}
	}

	for (uint32_t i = 0; i < RawInputButton::Count; ++i)
	{
		RawInputButton::Type button = static_cast<RawInputButton::Type>(i);
		std::string buttonString = ButtonConverter.ToString(button);
		
		std::string stateString = iniReader.ReadString("States", buttonString);

		if (InputState::CheckState(stateString))
		{
			InputState state = InputState::GetState(stateString);

			MapButtonToState(button, state);
		}
	}

	for (uint32_t i = 0; i < RawInputAxis::Count; ++i)
	{
		RawInputAxis::Type axis = static_cast<RawInputAxis::Type>(i);
		std::string axisString = AxisConverter.ToString(axis);
		
		std::string rangeString = iniReader.ReadString("Ranges", axisString);

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

	for (auto ii = m_actions.begin(); ii != m_actions.end(); ++ii)
	{
		RawInputButton::Type button = ii->first;
		InputAction action = ii->second;

		iniWriter.WriteString("Actions", ButtonConverter.ToString(button), action.ToString());
	}

	for (auto ii = m_states.begin(); ii != m_states.end(); ++ii)
	{
		RawInputButton::Type button = ii->first;
		InputState state = ii->second;

		iniWriter.WriteString("States", ButtonConverter.ToString(button), state.ToString());
	}

	for (auto ii = m_ranges.begin(); ii != m_ranges.end(); ++ii)
	{
		RawInputAxis::Type axis = ii->first;
		InputRange range = ii->second;

		iniWriter.WriteString("Ranges", AxisConverter.ToString(axis), range.ToString());
	}
}

InputAction InputContext::MapButtonToAction(RawInputButton::Type _button, InputAction _action)
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

InputState InputContext::MapButtonToState(RawInputButton::Type _button, InputState _state)
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

InputRange InputContext::MapAxisToRange(RawInputAxis::Type _axis, InputRange _range)
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