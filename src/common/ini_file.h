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

#ifndef SYLDEV_COMMON_INIFILE
#define SYLDEV_COMMON_INIFILE

#include <string>

namespace SylDev { namespace Common {

class IniFile
{
public:
	IniFile(std::string _file);

	int32_t ReadInt(std::string _section, std::string _key);
	void WriteInt(std::string _section, std::string _key, int32_t _value);

	float ReadFloat(std::string _section, std::string _key);
	void WriteFloat(std::string _section, std::string _key, float _value);

	bool ReadBool(std::string _section, std::string _key);
	void WriteBool(std::string _section, std::string _key, bool _value);

	std::string ReadString(std::string _section, std::string _key);
	void WriteString(std::string _section, std::string _key, std::string _value);

private:
	std::string m_file;
};

} } // SylDev, Common

#endif