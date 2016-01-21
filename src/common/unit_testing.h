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
#include <string>
#include <vector>
#include <unordered_map>

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
	TestSuite(TestEnvironment& _environment, std::string _name);

	std::string GetDependency(size_t _idx) const;
	size_t GetDependencyCount() const;

	void RegisterDependency(std::string _dependency);
	void RegisterUnitTest(UnitTest* _test);

	void AssignInit(std::function<void()> _func);
	void AssignClose(std::function<void()> _func);
	void AssignFixtureEnter(std::function<void()> _func);
	void AssignFixtureLeave(std::function<void()> _func);

	TestResult Run();

private:
	std::vector<std::string> m_dependencies;
	std::vector<UnitTest*> m_tests;

	std::function<void()> m_init;
	std::function<void()> m_close;
	std::function<void()> m_fixtureEnter;
	std::function<void()> m_fixtureLeave;
};

class RegisterDependency
{
public:
	RegisterDependency(TestSuite& _suite, std::string _dependency);
};

class AssignSuiteInit
{
public:
	AssignSuiteInit(TestSuite& _suite, std::function<void()> _func);
};

class AssignSuiteClose
{
public:
	AssignSuiteClose(TestSuite& _suite, std::function<void()> _func);
};

class AssignSuiteFixtureEnter
{
public:
	AssignSuiteFixtureEnter(TestSuite& _suite, std::function<void()> _func);
};

class AssignSuiteFixtureLeave
{
public:
	AssignSuiteFixtureLeave(TestSuite& _suite, std::function<void()> _func);
};

END_NAMESPACE

class TestEnvironment
{
	friend class Impl::TestSuite;

public:
	static TestEnvironment& GetInstance();

	TestResult Run();

private:
	void RegisterTestSuite(std::string _name, Impl::TestSuite* _suite);

	bool SolveDependencies(std::vector<Impl::TestSuite*>& _sorted);
	bool TopologicalSortDependencies(
		std::vector<Impl::TestSuite*>& _sorted, std::vector<std::vector<size_t>> const& _dependencies);
	bool Visit(
		std::vector<Impl::TestSuite*>& _sorted, std::vector<std::vector<size_t>> const& _dependencies, 
		size_t _idx, std::vector<bool>& _tempmarked, std::vector<bool>& _marked);

	static TestEnvironment* Instance;

	std::unordered_map<std::string, size_t> m_names;
	std::vector<Impl::TestSuite*> m_suites;
	std::vector<TestResult> m_results;
};

END_2_NAMESPACES

#define SYLDEV_TESTSUITE(name) \
BEGIN_NAMESPACE(name) \
static SylDev::Common::Impl::TestSuite Suite(SylDev::Common::TestEnvironment::GetInstance(), #name); \
END_NAMESPACE \
namespace name \

#define SYLDEV_TESTSUITE_DEPEND(name) \
static SylDev::Common::Impl::RegisterDependency Dependency_##name(Suite, #name); \

#define SYLDEV_UNITTEST(name) \
void Func_##name(SylDev::Common::TestResult& _result); \
static SylDev::Common::Impl::UnitTest Test_##name(Suite, Func_##name); \
void Func_##name(SylDev::Common::TestResult& _result) \

#define SYLDEV_TESTSUITE_INIT \
void Func_Init(); \
static SylDev::Common::Impl::AssignSuiteInit Assign_Init(Suite, Func_Init); \
void Func_Init() \

#define SYLDEV_TESTSUITE_CLOSE \
void Func_Close(); \
static SylDev::Common::Impl::AssignSuiteClose Assign_Close(Suite, Func_Close); \
void Func_Close() \

#define SYLDEV_TESTSUITE_FIXTURE_ENTER \
void Func_FixtureEnter(); \
static SylDev::Common::Impl::AssignSuiteFixtureEnter Assign_FixtureEnter(Suite, Func_FixtureEnter); \
void Func_FixtureEnter() \

#define SYLDEV_TESTSUITE_FIXTURE_LEAVE \
void Func_FixtureLeave(); \
static SylDev::Common::Impl::AssignSuiteFixtureLeave Assign_FixtureLeave(Suite, Func_FixtureLeave); \
void Func_FixtureLeave() \

#endif