#include "input_types.h"

#include <assert.h>

using namespace SylDev::Framework;

std::unordered_map<size_t, std::string> InputAction::Actions;
std::unordered_map<size_t, std::string> InputState::States;
std::unordered_map<size_t, std::string> InputRange::Ranges;

bool InputAction::CheckAction(std::string _name)
{
	std::hash<std::string> hash;

	size_t id = hash(_name);
	auto ii = Actions.find(id);

	if (ii != Actions.end())
	{
		std::string name = ii->second;
		assert(name == _name);

		return true;
	}

	return false;
}

InputAction InputAction::GetAction(std::string _name)
{
	return InputAction(_name);
}

std::string InputAction::ToString() const
{
	auto ii = Actions.find(m_id);

	if (ii != Actions.end())
	{
		return ii->second;
	}

	return "";
}

InputAction::InputAction(std::string _name)
{
	std::hash<std::string> hash;

	m_id = hash(_name);
	auto ii = Actions.emplace(m_id, _name);

	if (!ii.second) // There was already an element associated with the calculated id.
	{
		std::string name = ii.first->second;
		assert(name == _name);
	}
}

bool InputState::CheckState(std::string _name)
{
	std::hash<std::string> hash;

	size_t id = hash(_name);
	auto ii = States.find(id);

	if (ii != States.end())
	{
		std::string name = ii->second;
		assert(name == _name);

		return true;
	}

	return false;
}

InputState InputState::GetState(std::string _name)
{
	return InputState(_name);
}

std::string InputState::ToString() const
{
	auto ii = States.find(m_id);

	if (ii != States.end())
	{
		return ii->second;
	}

	return "";
}

InputState::InputState(std::string _name)
{
	std::hash<std::string> hash;

	m_id = hash(_name);
	auto ii = States.emplace(m_id, _name);

	if (!ii.second) // There was already an element associated with the calculated id.
	{
		std::string name = ii.first->second;
		assert(name == _name);
	}
}

bool InputRange::CheckRange(std::string _name)
{
	std::hash<std::string> hash;

	size_t id = hash(_name);
	auto ii = Ranges.find(id);

	if (ii != Ranges.end())
	{
		std::string name = ii->second;
		assert(name == _name);

		return true;
	}

	return false;
}

InputRange InputRange::GetRange(std::string _name)
{
	return InputRange(_name);
}

std::string InputRange::ToString() const
{
	auto ii = Ranges.find(m_id);

	if (ii != Ranges.end())
	{
		return ii->second;
	}

	return "";
}

InputRange::InputRange(std::string _name)
{
	std::hash<std::string> hash;

	m_id = hash(_name);
	auto ii = Ranges.emplace(m_id, _name);

	if (!ii.second) // There was already an element associated with the calculated id.
	{
		std::string name = ii.first->second;
		assert(name == _name);
	}
}