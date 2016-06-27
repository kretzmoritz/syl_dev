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

#ifndef SYLDEV_FRAMEWORK_INPUTSYSTEM
#define SYLDEV_FRAMEWORK_INPUTSYSTEM

#include <string>
#include <stack>
#include <map>
#include <set>

#include "raw_input_handler.h"

namespace SylDev { namespace Framework {

template<class InputContext>
class InputSystem
{
	struct Context
	{
	public:
		Context(InputContext const& _context, bool _active);

		InputContext const& Get() const;

		bool IsActive() const;
		void SetActive(bool _active);

	private:
		InputContext const& m_context;
		bool m_active;
	};

public:
	InputSystem(RawInputHandler const& _rawInputHandler);

	void PushState();
	void PopState();

	void AddContext(std::string _name, InputContext const& _context);
	void RemoveContext(std::string _name);

	void ActivateContext(std::string _name);
	void DeactivateContext(std::string _name);

	void Update();

	bool Check(typename InputContext::ActionType _action) const;
	bool Check(typename InputContext::StateType _state) const;
	bool Check(typename InputContext::RangeType _range, float& _delta) const;

private:
	RawInputHandler const& m_rawInputHandler;

	std::stack<std::map<std::string, Context>> m_contexts;

	std::set<typename InputContext::ActionType> m_activeActions;
	std::set<typename InputContext::StateType> m_activeStates;
	std::map<typename InputContext::RangeType, float> m_activeRanges;
};

} } // SylDev, Framework

#include "input_system.tcc"

#endif