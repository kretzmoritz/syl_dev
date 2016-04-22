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

#ifndef SYLDEV_FRAMEWORK_INPUTTYPES
#define SYLDEV_FRAMEWORK_INPUTTYPES

#include <unordered_map>
#include <string>

#include "../common/namespace.h"

BEGIN_2_NAMESPACES(SylDev, Framework)

class InputAction
{
public:
	std::string GetIdentifier() const;

	static bool CheckAction(std::string _identifier);
	static InputAction GetAction(std::string _identifier);

private:
	InputAction(std::string _identifier);

	std::string m_identifier;

	static std::unordered_map<std::string, InputAction> Actions;
};

class InputState
{
public:
	std::string GetIdentifier() const;

	static bool CheckState(std::string _identifier);
	static InputState GetState(std::string _identifier);

private:
	InputState(std::string _identifier);

	std::string m_identifier;

	static std::unordered_map<std::string, InputState> States;
};

class InputRange
{
public:
	std::string GetIdentifier() const;

	static bool CheckRange(std::string _identifier);
	static InputRange GetRange(std::string _identifier);

private:
	InputRange(std::string _identifier);

	std::string m_identifier;

	static std::unordered_map<std::string, InputRange> Ranges;
};

END_2_NAMESPACES

#endif