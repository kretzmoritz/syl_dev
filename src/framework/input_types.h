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

namespace SylDev { namespace Framework {

class InputAction
{
	static std::unordered_map<size_t, std::string> Actions;

public:
	static bool CheckAction(std::string _name);
	static InputAction GetAction(std::string _name);

	std::string ToString() const;

private:
	InputAction(std::string _name);

	size_t m_id;
};

class InputState
{
	static std::unordered_map<size_t, std::string> States;

public:
	static bool CheckState(std::string _name);
	static InputState GetState(std::string _name);

	std::string ToString() const;

private:
	InputState(std::string _name);

	size_t m_id;
};

class InputRange
{
	static std::unordered_map<size_t, std::string> Ranges;

public:
	static bool CheckRange(std::string _name);
	static InputRange GetRange(std::string _name);

	std::string ToString() const;

private:
	InputRange(std::string _name);

	size_t m_id;
};

} } // SylDev, Framework

#endif