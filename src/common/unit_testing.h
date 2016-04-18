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
#include <sstream>
#include <vector>
#include <unordered_map>

#include "namespace.h"

BEGIN_2_NAMESPACES(SylDev, Common)

class TestEnvironment;

BEGIN_NAMESPACE(TestData)

class TestInfo
{
public:
	TestInfo(std::string _file, int _line);

	std::string m_file;
	int m_line;
};

class AssertResult
{
public:
	AssertResult(TestInfo const& _info, std::string _message, bool _result, bool _internal = false);

	TestInfo m_info;
	std::string m_message;
	bool m_result;
	bool m_internal;
};

class TestResult
{
public:
	TestResult(std::string _name);

	void AddResult(AssertResult const& _assertResult);

	std::string GetName() const;
	std::vector<AssertResult> const& GetResults() const;
	bool GetTotalResult() const;

private:
	std::string m_name;
	std::vector<AssertResult> m_assertResults;
	bool m_totalResult;
};

class SuiteResult
{
public:
	SuiteResult(std::string _name);

	void Clear();
	void AddResult(TestResult const& _testResult);

	std::string GetName() const;
	std::vector<TestResult> const& GetResults() const;
	bool GetTotalResult() const;

private:
	std::string m_name;
	std::vector<TestResult> m_testResults;
	bool m_totalResult;
};

END_NAMESPACE

class TestContext
{
public:
	TestContext(TestData::TestResult& _testResult);

	void AddResult(TestData::TestInfo const& _info, std::string _message, bool _succeeded);

private:
	TestData::TestResult& m_testResult;
};

BEGIN_NAMESPACE(Impl)

class TestSuite;

class UnitTest
{
public:
	UnitTest(TestSuite& _suite, std::string _name, std::function<void(TestContext&)> _func);

	std::string GetName() const;

	void Run(TestData::TestResult& _testResult);

private:
	std::string m_name;
	std::function<void(TestContext&)> m_func;
};

class TestDependency
{
public:
	TestDependency(TestData::TestInfo const& _info, std::string _name);

	TestData::TestInfo m_info;
	std::string m_name;
};

class TestSuite
{
public:
	TestSuite(TestEnvironment& _environment, std::string _name);

	void RegisterDependency(TestData::TestInfo const& _info, std::string _dependency);
	void RegisterUnitTest(UnitTest* _test);

	void AssignInit(std::function<void()> _func);
	void AssignClose(std::function<void()> _func);
	void AssignFixtureEnter(std::function<void()> _func);
	void AssignFixtureLeave(std::function<void()> _func);

	std::string GetName() const;

	TestDependency GetDependency(size_t _idx) const;
	size_t GetDependencyCount() const;

	void Run(TestData::SuiteResult& _suiteResult);

private:
	std::string m_name;
	std::vector<TestDependency> m_dependencies;
	std::vector<UnitTest*> m_tests;

	std::function<void()> m_init;
	std::function<void()> m_close;
	std::function<void()> m_fixtureEnter;
	std::function<void()> m_fixtureLeave;
};

class RegisterDependency
{
public:
	RegisterDependency(TestSuite& _suite, TestData::TestInfo const& _info, std::string _dependency);
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

class TestPrinter
{
public:
	virtual ~TestPrinter() = default;

	virtual void OnEnd(std::vector<TestData::SuiteResult> const& _suiteResults) = 0;
};

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

class TestEnvironment
{
	friend class Impl::TestSuite;

public:
	static TestEnvironment& GetInstance();

	TestEnvironment();

	void AssignPrinter(TestPrinter* _printer);
	bool Run();

private:
	void RegisterTestSuite(std::string _name, Impl::TestSuite* _suite);

	bool SolveDependencies(std::vector<Impl::TestSuite*>& _sorted);
	bool TopologicalSortDependencies(
		std::vector<Impl::TestSuite*>& _sorted, std::vector<std::vector<std::pair<size_t, size_t>>> const& _dependencies);
	bool TopologicalVisit(
		std::vector<Impl::TestSuite*>& _sorted, std::vector<std::vector<std::pair<size_t, size_t>>> const& _dependencies, 
		size_t _idx, std::vector<bool>& _tempmarked, std::vector<bool>& _marked);

	static TestEnvironment* Instance;

	TestPrinter* m_printer;

	std::unordered_map<std::string, size_t> m_names;
	std::vector<Impl::TestSuite*> m_suites;
	std::vector<TestData::SuiteResult> m_suiteResults;
};

END_2_NAMESPACES

#define SYLDEV_TESTSUITE(name) \
BEGIN_NAMESPACE(name) \
static SylDev::Common::Impl::TestSuite Suite(SylDev::Common::TestEnvironment::GetInstance(), #name); \
END_NAMESPACE \
namespace name \

#define SYLDEV_TESTSUITE_DEPEND(name) \
static SylDev::Common::Impl::RegisterDependency Dependency_##name(Suite, SylDev::Common::TestData::TestInfo(__FILE__, __LINE__), #name); \

#define SYLDEV_UNITTEST(name) \
void Func_##name(SylDev::Common::TestContext& _ctx); \
static SylDev::Common::Impl::UnitTest Test_##name(Suite, #name, Func_##name); \
void Func_##name(SylDev::Common::TestContext& _ctx) \

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

#define SYLDEV_UNITTEST_ASSERT(cond) \
do \
{ \
	_ctx.AddResult(SylDev::Common::TestData::TestInfo(__FILE__, __LINE__), #cond, (cond)); \
} while (false) \

#define SYLDEV_UNITTEST_ASSERT_EX(cond, msg) \
do \
{ \
	_ctx.AddResult(SylDev::Common::TestData::TestInfo(__FILE__, __LINE__), msg, (cond)); \
} while (false) \

#endif