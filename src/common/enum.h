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

#ifndef SYLDEV_COMMON_ENUM
#define SYLDEV_COMMON_ENUM

#include <vector>
#include <string>
#include <sstream>

#define ENUM_WITH_STRING(name, ...) \
	struct name \
	{ \
		enum Type \
		{ \
			__VA_ARGS__, \
			Count \
		}; \
		class Converter \
		{ \
		public: \
			Converter() \
			{ \
				int32_t const test[] = { __VA_ARGS__ }; \
				std::string enumName = #name; \
				std::string params = #__VA_ARGS__; \
				size_t length = params.length(); \
				std::ostringstream stream; \
				for (size_t i = 0; i < length; ++i) \
				{ \
					if (isspace(params[i])) \
					{ \
						continue; \
					} \
					else if (params[i] == ',') \
					{ \
						std::string string = stream.str(); \
						if (!string.empty()) \
						{ \
							m_strings.push_back(enumName + "::" + string); \
							stream.str(""); \
							stream.clear(); \
						} \
					} \
					else \
					{ \
						stream << params[i]; \
					} \
				} \
				std::string string = stream.str(); \
				if (!string.empty()) \
				{ \
					m_strings.push_back(enumName + "::" + string); \
				} \
			} \
			std::string ToString(Type _value) \
			{ \
				if (_value < m_strings.size()) \
				{ \
					return m_strings[_value]; \
				} \
				return ""; \
			} \
		private: \
			std::vector<std::string> m_strings; \
		}; \
	}; \

#endif