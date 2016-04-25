#include "input_context.h"

#include "../common/ini_file.h"

using namespace SylDev::Framework;
using namespace SylDev::Common;

RawInputButton::Converter InputContext::ButtonConverter;
RawInputAxis::Converter InputContext::AxisConverter;

void InputContext::ReadFromFile(std::string _file)
{
	m_actions.clear();
	m_states.clear();
	m_ranges.clear();

	IniFile iniReader(_file);

	for (uint32_t i = 0; i < RawInputButton::Count; ++i)
	{
		RawInputButton::Type button = static_cast<RawInputButton::Type>(i);
		std::string actionString = iniReader.ReadString("Actions", ButtonConverter.ToString(button));

		if (InputAction::CheckAction(actionString))
		{
			InputAction action = InputAction::GetAction(actionString);

			MapButtonToAction(button, action);
		}
	}

	for (uint32_t i = 0; i < RawInputButton::Count; ++i)
	{
		RawInputButton::Type button = static_cast<RawInputButton::Type>(i);
		std::string stateString = iniReader.ReadString("States", ButtonConverter.ToString(button));

		if (InputState::CheckState(stateString))
		{
			InputState state = InputState::GetState(stateString);

			MapButtonToState(button, state);
		}
	}

	for (uint32_t i = 0; i < RawInputAxis::Count; ++i)
	{
		RawInputAxis::Type axis = static_cast<RawInputAxis::Type>(i);
		std::string rangeString = iniReader.ReadString("Ranges", AxisConverter.ToString(axis));

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

	for (uint32_t i = 0; i < RawInputButton::Count; ++i)
	{
		RawInputButton::Type button = static_cast<RawInputButton::Type>(i);
		std::string actionString = "";

		auto ii = m_actions.find(button);

		if (ii != m_actions.end())
		{
			actionString = ii->second.ToString();
		}

		iniWriter.WriteString("Actions", ButtonConverter.ToString(button), actionString);
	}

	for (uint32_t i = 0; i < RawInputButton::Count; ++i)
	{
		RawInputButton::Type button = static_cast<RawInputButton::Type>(i);
		std::string stateString = "";

		auto ii = m_states.find(button);

		if (ii != m_states.end())
		{
			stateString = ii->second.ToString();
		}

		iniWriter.WriteString("States", ButtonConverter.ToString(button), stateString);
	}

	for (uint32_t i = 0; i < RawInputAxis::Count; ++i)
	{
		RawInputAxis::Type axis = static_cast<RawInputAxis::Type>(i);
		std::string rangeString = "";

		auto ii = m_ranges.find(axis);

		if (ii != m_ranges.end())
		{
			rangeString = ii->second.ToString();
		}

		iniWriter.WriteString("Ranges", AxisConverter.ToString(axis), rangeString);
	}
}

InputAction InputContext::MapButtonToAction(RawInputButton::Type _button, InputAction _action)
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

InputState InputContext::MapButtonToState(RawInputButton::Type _button, InputState _state)
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

InputRange InputContext::MapAxisToRange(RawInputAxis::Type _axis, InputRange _range)
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