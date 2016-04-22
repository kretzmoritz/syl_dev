#include "input_types.h"

using namespace SylDev::Framework;

std::string InputAction::GetIdentifier() const
{
	return m_identifier;
}

bool InputAction::CheckAction(std::string _identifier)
{
	return Actions.find(_identifier) != Actions.end();
}

InputAction InputAction::GetAction(std::string _identifier)
{
	auto ii = Actions.emplace(_identifier, _identifier);

	return ii.first->second;
}

InputAction::InputAction(std::string _identifier)
	: m_identifier(_identifier)
{
}

std::string InputState::GetIdentifier() const
{
	return m_identifier;
}

bool InputState::CheckState(std::string _identifier)
{
	return States.find(_identifier) != States.end();
}

InputState InputState::GetState(std::string _identifier)
{
	auto ii = States.emplace(_identifier, _identifier);

	return ii.first->second;
}

InputState::InputState(std::string _identifier)
	: m_identifier(_identifier)
{
}

std::string InputRange::GetIdentifier() const
{
	return m_identifier;
}

bool InputRange::CheckRange(std::string _identifier)
{
	return Ranges.find(_identifier) != Ranges.end();
}

InputRange InputRange::GetRange(std::string _identifier)
{
	auto ii = Ranges.emplace(_identifier, _identifier);

	return ii.first->second;
}

InputRange::InputRange(std::string _identifier)
	: m_identifier(_identifier)
{
}