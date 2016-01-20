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

#ifndef SYLDEV_COMMON_UNITTESTING
#define SYLDEV_COMMON_UNITTESTING

#include <functional>
#include <vector>

#include "namespace.h"

BEGIN_2_NAMESPACES(SylDev, Common)

class TestEnvironment;

enum class TestResult
{
	Unknown = 0,
	Failed = -1,
	Succeeded = 1,
};

BEGIN_NAMESPACE(Impl)

class TestSuite;

class UnitTest
{
public:
	UnitTest(TestSuite& _suite, std::function<void(TestResult&)> _func);

	TestResult Run();

private:
	std::function<void(TestResult&)> m_func;
};

class TestSuite
{
public:
	TestSuite(TestEnvironment& _environment);

	void RegisterUnitTest(UnitTest* _test);
	TestResult Run();

private:
	std::vector<UnitTest*> m_tests;
};

END_NAMESPACE

class TestEnvironment
{
public:
	static TestEnvironment& GetInstance();

	void RegisterTestSuite(Impl::TestSuite* _suite);
	TestResult Run();

private:
	static TestEnvironment* Instance;

	std::vector<Impl::TestSuite*> m_suites;
};

//Suite
//{
	//SuiteInit
	
	//PreTest
	//Test
	//PostTest

	//PreTest
	//Test
	//PostTest

	//PreTest
	//Test
	//PostTest

	//SuiteClose
//}

END_2_NAMESPACES

#define SYLDEV_TESTSUITE(name) \
BEGIN_NAMESPACE(name) \
static SylDev::Common::Impl::TestSuite Suite(SylDev::Common::TestEnvironment::GetInstance()); \
END_NAMESPACE \
namespace name \

#define SYLDEV_UNITTEST(name) \
void Func_##name(SylDev::Common::TestResult& _result); \
static SylDev::Common::Impl::UnitTest Test_##name(Suite, Func_##name); \
void Func_##name(SylDev::Common::TestResult& _result) \

#endif