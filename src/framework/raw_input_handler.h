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

#include "../common/namespace.h"
#include "../math/vector.h"

BEGIN_2_NAMESPACES(SylDev, Framework)

class RawInputHandler
{
	static const unsigned int KeyCount = 256;

public:
	RawInputHandler();

	void Refresh();

	bool IsPressed(unsigned int _id);
	bool IsDown(unsigned int _id);
	bool IsToggled(unsigned int _id);
	bool IsReleased(unsigned int _id);

	SylDev::Math::Vec2i GetMousePosLocal() const;
	SylDev::Math::Vec2i GetMousePosScreen() const;
	bool IsMouseInWindow() const;

	bool HasFocus() const;

private:
	bool m_pressed[KeyCount];
	bool m_down[KeyCount];
	bool m_toggled[KeyCount];
	bool m_released[KeyCount];
};

END_2_NAMESPACES

#endif