#include "unit_testing.h"

using namespace SylDev::Common;
using namespace Impl;

TestEnvironment* TestEnvironment::Instance = nullptr;

UnitTest::UnitTest(TestSuite& _suite, std::function<void(TestResult&)> _func)
	: m_func(std::move(_func))
{
	_suite.RegisterUnitTest(this);
}

TestResult UnitTest::Run()
{
	TestResult result = TestResult::Succeeded;
	m_func(result);

	return result;
}

TestSuite::TestSuite(TestEnvironment& _environment)
{
	_environment.RegisterTestSuite(this);
}

void TestSuite::RegisterUnitTest(UnitTest* _test)
{
	m_tests.push_back(_test);
}

TestResult TestSuite::Run()
{
	TestResult totalResult = TestResult::Succeeded;

	for (auto it = m_tests.begin(); it != m_tests.end(); ++it)
	{
		TestResult result = (*it)->Run();

		if (totalResult == TestResult::Failed)
		{
			continue;
		}

		if (result == TestResult::Failed || result == TestResult::Unknown)
		{
			totalResult = result;
		}
	}

	return totalResult;
}

TestEnvironment& TestEnvironment::GetInstance()
{
	if (!Instance)
	{
		Instance = new TestEnvironment();
	}

	return *Instance;
}

void TestEnvironment::RegisterTestSuite(TestSuite* _suite)
{
	m_suites.push_back(_suite);
}

TestResult TestEnvironment::Run()
{
	TestResult totalResult = TestResult::Succeeded;

	for (auto it = m_suites.begin(); it != m_suites.end(); ++it)
	{
		TestResult result = (*it)->Run();

		if (totalResult == TestResult::Failed)
		{
			continue;
		}

		if (result == TestResult::Failed || result == TestResult::Unknown)
		{
			totalResult = result;
		}
	}

	return totalResult;
}