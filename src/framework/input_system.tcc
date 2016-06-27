#include "input_system.h"

namespace SylDev { namespace Framework {

template<class InputContext>
InputSystem<InputContext>::Context::Context(InputContext const& _context, bool _active)
	: m_context(_context), m_active(_active)
{
}

template<class InputContext>
InputContext const& InputSystem<InputContext>::Context::Get() const
{
	return m_context;
}

template<class InputContext>
bool InputSystem<InputContext>::Context::IsActive() const
{
	return m_active;
}

template<class InputContext>
void InputSystem<InputContext>::Context::SetActive(bool _active)
{
	m_active = _active;
}

template<class InputContext>
InputSystem<InputContext>::InputSystem(RawInputHandler const& _rawInputHandler)
	: m_rawInputHandler(_rawInputHandler),
	m_lastMousePos(_rawInputHandler.GetMousePosLocal())
{
	m_contexts.push(std::map<std::string, Context>());
}

template<class InputContext>
void InputSystem<InputContext>::PushState()
{
	m_contexts.push(m_contexts.top());
}

template<class InputContext>
void InputSystem<InputContext>::PopState()
{
	if (m_contexts.size() > 1)
	{
		m_contexts.pop();
	}
}

template<class InputContext>
void InputSystem<InputContext>::AddContext(std::string _name, InputContext const& _context)
{
	auto& contexts = m_contexts.top();

	contexts.emplace(std::piecewise_construct, std::forward_as_tuple(_name), std::forward_as_tuple(_context, false));
}

template<class InputContext>
void InputSystem<InputContext>::RemoveContext(std::string _name)
{
	auto& contexts = m_contexts.top();

	contexts.erase(_name);
}

template<class InputContext>
void InputSystem<InputContext>::ActivateContext(std::string _name)
{
	auto& contexts = m_contexts.top();

	auto ii = contexts.find(_name);

	if (ii != contexts.end())
	{
		ii->second.SetActive(true);
	}
}

template<class InputContext>
void InputSystem<InputContext>::DeactivateContext(std::string _name)
{
	auto& contexts = m_contexts.top();

	auto ii = contexts.find(_name);

	if (ii != contexts.end())
	{
		ii->second.SetActive(false);
	}
}

template<class InputContext>
void InputSystem<InputContext>::Update()
{
	m_activeActions.clear();
	m_activeStates.clear();
	m_activeRanges.clear();

	// update mouse delta
	Math::Vec2i currentMousePos = m_rawInputHandler.GetMousePosLocal();
	Math::Vec2i mouseDelta = currentMousePos - m_lastMousePos;
	m_lastMousePos = currentMousePos;

	if (!m_rawInputHandler.HasFocus())
	{
		return;
	}

	for (auto& element : m_contexts.top())
	{
		Context& context = element.second;

		if (!context.IsActive())
		{
			continue;
		}

		InputContext const& inputContext = context.Get();

		for (auto& mapping : inputContext.GetActionMapping())
		{
			RawInputButton button = mapping.first;
			InputContext::ActionType action = mapping.second;

			if (m_rawInputHandler.IsPressed(button))
			{
				m_activeActions.emplace(action);
			}
		}

		for (auto& mapping : inputContext.GetStateMapping())
		{
			RawInputButton button = mapping.first;
			InputContext::StateType state = mapping.second;

			if (m_rawInputHandler.IsDown(button))
			{
				m_activeStates.emplace(state);
			}
		}

		for (auto& mapping : inputContext.GetRangeMapping())
		{
			RawInputAxis axis = mapping.first;
			InputContext::RangeType range = mapping.second;

			switch (axis)
			{
			case RawInputAxis::MouseX:
				if (m_rawInputHandler.IsMouseInWindow() && mouseDelta.x != 0)
				{
					m_activeRanges.emplace(range, static_cast<float>(mouseDelta.x));
				}
				break;
			case RawInputAxis::MouseY:
				if (m_rawInputHandler.IsMouseInWindow() && mouseDelta.y != 0)
				{
					m_activeRanges.emplace(range, static_cast<float>(mouseDelta.y));
				}
				break;
			case RawInputAxis::MouseScroll:
				break;
			}
		}
	}
}

template<class InputContext>
bool InputSystem<InputContext>::Check(typename InputContext::ActionType _action) const
{
	return m_activeActions.find(_action) != m_activeActions.end();
}

template<class InputContext>
bool InputSystem<InputContext>::Check(typename InputContext::StateType _state) const
{
	return m_activeStates.find(_state) != m_activeStates.end();
}

template<class InputContext>
bool InputSystem<InputContext>::Check(typename InputContext::RangeType _range, float& _delta) const
{
	auto ii = m_activeRanges.find(_range);

	if (ii != m_activeRanges.end())
	{
		_delta = ii->second;

		return true;
	}

	_delta = 0.0f;

	return false;
}

} } // SylDev, Framework