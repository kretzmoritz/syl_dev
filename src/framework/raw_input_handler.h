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

#include "raw_input_constants.h"
#include "../math/vector.h"

namespace SylDev { namespace Framework {

class RawInputHandler
{
	static const uint32_t KeyCount = 256;
	static const std::map<RawInputButton, uint32_t> Mapping;

public:
	RawInputHandler();

	void Update();

	bool IsPressed(RawInputButton _button) const;
	bool IsDown(RawInputButton _button) const;
	bool IsToggled(RawInputButton _button) const;
	bool IsReleased(RawInputButton _button) const;

	Math::Vec2i GetMousePosLocal() const;
	Math::Vec2i GetMousePosScreen() const;

	void AddMouseWheelDelta(int32_t _delta);
	int32_t GetMouseWheelDelta() const;

	bool HasFocus() const;
	bool IsMouseInWindow() const;

private:
	bool GetId(RawInputButton _button, uint32_t& _id) const;

	bool m_pressed[KeyCount];
	bool m_down[KeyCount];
	bool m_toggled[KeyCount];
	bool m_released[KeyCount];

	int32_t m_currentMouseWheelDelta;
	int32_t m_lastMouseWheelDelta;
};

} } // SylDev, Framework

#endif