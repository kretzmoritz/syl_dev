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

#ifndef SYLDEV_FRAMEWORK_RAWINPUTHANDLER
#define SYLDEV_FRAMEWORK_RAWINPUTHANDLER

#include <map>

#include "../common/namespace.h"
#include "raw_input_constants.h"
#include "../math/vector.h"

BEGIN_2_NAMESPACES(SylDev, Framework)

class RawInputHandler
{
	static const unsigned int KeyCount = 256;
	static const std::map<RawInputButton::Type, unsigned int> Mapping;

public:
	RawInputHandler();

	void Refresh();

	bool IsPressed(RawInputButton::Type _button);
	bool IsDown(RawInputButton::Type _button);
	bool IsToggled(RawInputButton::Type _button);
	bool IsReleased(RawInputButton::Type _button);

	SylDev::Math::Vec2i GetMousePosLocal() const;
	SylDev::Math::Vec2i GetMousePosScreen() const;
	bool IsMouseInWindow() const;

	bool HasFocus() const;

private:
	bool GetId(RawInputButton::Type _button, unsigned int& _id) const;

	bool m_pressed[KeyCount];
	bool m_down[KeyCount];
	bool m_toggled[KeyCount];
	bool m_released[KeyCount];
};

END_2_NAMESPACES

#endif