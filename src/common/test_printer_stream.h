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

#ifndef SYLDEV_COMMON_TESTPRINTERSTREAM
#define SYLDEV_COMMON_TESTPRINTERSTREAM

#include <string>
#include <sstream>

#include "unit_testing.h"

namespace SylDev { namespace Common {

class TestPrinterStream
	: public TestPrinter
{
public:
	virtual ~TestPrinterStream() = default;

	void OnEnd(std::vector<TestData::SuiteResult> const& _suiteResults);

	void OutputToConsole();
	void OutputToFile(std::string _file);

private:
	std::stringstream m_stream;
};

} } // SylDev, Common

#endif