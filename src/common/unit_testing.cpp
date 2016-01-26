#include "unit_testing.h"

using namespace SylDev::Common;
using namespace Impl;

TestEnvironment* TestEnvironment::Instance = nullptr;

TestInfo::TestInfo(std::string _file, int _line)
	: m_file(_file), m_line(_line)
{
}

AssertResult::AssertResult(TestInfo const& _info, std::string _message, bool _result)
	: m_info(_info), m_message(_message), m_result(_result)
{
}

TestResult::TestResult(std::string _name)
	: m_name(_name), m_totalResult(true)
{
}

void TestResult::AddResult(AssertResult const& _assertResult)
{
	m_assertResults.push_back(_assertResult);

	if (!_assertResult.m_result)
	{
		m_totalResult = false;
	}
}

bool TestResult::GetTotalResult() const
{
	return m_totalResult;
}

TestContext::TestContext(TestResult& _testResult)
	: m_testResult(_testResult)
{
}

void TestContext::AddResult(TestInfo const& _info, std::string _message, bool _succeeded)
{
	m_testResult.AddResult(AssertResult(_info, _message, _succeeded));
}

SuiteResult::SuiteResult(std::string _name)
	: m_name(_name), m_totalResult(true)
{
}

void SuiteResult::AddResult(TestResult const& _testResult)
{
	m_testResults.push_back(_testResult);

	if (!_testResult.GetTotalResult())
	{
		m_totalResult = false;
	}
}

bool SuiteResult::GetTotalResult() const
{
	return m_totalResult;
}

void SuiteResult::SetTotalResult(bool _totalResult)
{
	m_totalResult = _totalResult;
}

UnitTest::UnitTest(TestSuite& _suite, std::string _name, std::function<void(TestContext&)> _func)
	: m_name(_name), m_func(std::move(_func))
{
	_suite.RegisterUnitTest(this);
}

std::string UnitTest::GetName() const
{
	return m_name;
}

void UnitTest::Run(TestResult& _testResult)
{
	TestContext ctx(_testResult);
	m_func(ctx);
}

TestSuite::TestSuite(TestEnvironment& _environment, std::string _name)
	: m_name(_name)
{
	_environment.RegisterTestSuite(_name, this);
}

void TestSuite::RegisterDependency(std::string _dependency)
{
	m_dependencies.push_back(_dependency);
}

void TestSuite::RegisterUnitTest(UnitTest* _test)
{
	m_tests.push_back(_test);
}

void TestSuite::AssignInit(std::function<void()> _func)
{
	m_init = std::move(_func);
}

void TestSuite::AssignClose(std::function<void()> _func)
{
	m_close = std::move(_func);
}

void TestSuite::AssignFixtureEnter(std::function<void()> _func)
{
	m_fixtureEnter = std::move(_func);
}

void TestSuite::AssignFixtureLeave(std::function<void()> _func)
{
	m_fixtureLeave = std::move(_func);
}

std::string TestSuite::GetName() const
{
	return m_name;
}

std::string TestSuite::GetDependency(size_t _idx) const
{
	return m_dependencies[_idx];
}

size_t TestSuite::GetDependencyCount() const
{
	return m_dependencies.size();
}

void TestSuite::Run(SuiteResult& _suiteResult)
{
	if (m_init)
	{
		m_init();
	}

	for (auto it = m_tests.begin(); it != m_tests.end(); ++it)
	{
		UnitTest* test = *it;

		if (m_fixtureEnter)
		{
			m_fixtureEnter();
		}

		TestResult testResult(test->GetName());
		test->Run(testResult);

		_suiteResult.AddResult(testResult);

		if (m_fixtureLeave)
		{
			m_fixtureLeave();
		}
	}

	if (m_close)
	{
		m_close();
	}
}

RegisterDependency::RegisterDependency(TestSuite& _suite, std::string _dependency)
{
	_suite.RegisterDependency(_dependency);
}

AssignSuiteInit::AssignSuiteInit(TestSuite& _suite, std::function<void()> _func)
{
	_suite.AssignInit(_func);
}

AssignSuiteClose::AssignSuiteClose(TestSuite& _suite, std::function<void()> _func)
{
	_suite.AssignClose(_func);
}

AssignSuiteFixtureEnter::AssignSuiteFixtureEnter(TestSuite& _suite, std::function<void()> _func)
{
	_suite.AssignFixtureEnter(_func);
}

AssignSuiteFixtureLeave::AssignSuiteFixtureLeave(TestSuite& _suite, std::function<void()> _func)
{
	_suite.AssignFixtureLeave(_func);
}

TestEnvironment& TestEnvironment::GetInstance()
{
	if (!Instance)
	{
		Instance = new TestEnvironment();
	}

	return *Instance;
}

bool TestEnvironment::Run()
{
	bool totalResult = true;

	std::vector<TestSuite*> sorted_suites;
	bool succeeded = SolveDependencies(sorted_suites);

	if (!succeeded)
	{
		// Resolve error

		return false;
	}

	for (size_t i = 0; i < sorted_suites.size(); ++i)
	{
		bool DependencyError = false;

		for (size_t j = 0; j < sorted_suites[i]->GetDependencyCount(); ++j)
		{
			auto it = m_names.find(sorted_suites[i]->GetDependency(j));

			if (it != m_names.end())
			{
				size_t idx = it->second;

				if (!m_suiteResults[idx].GetTotalResult())
				{
					// Dependant test has failed error

					DependencyError = true;
				}
			}
		}

		auto it = m_names.find(sorted_suites[i]->GetName());

		if (it != m_names.end())
		{
			size_t idx = it->second;

			if (DependencyError)
			{
				m_suiteResults[idx].SetTotalResult(false);
			}
			else
			{
				sorted_suites[i]->Run(m_suiteResults[idx]);
			}

			if (!m_suiteResults[idx].GetTotalResult())
			{
				totalResult = false;
			}
		}
	}

	return totalResult;
}

void TestEnvironment::RegisterTestSuite(std::string _name, TestSuite* _suite)
{
	m_names.emplace(_name, m_suites.size());
	m_suites.push_back(_suite);
	m_suiteResults.push_back(SuiteResult(_name));
}

bool TestEnvironment::SolveDependencies(std::vector<TestSuite*>& _sorted)
{
	std::vector<std::vector<size_t>> dependencies;

	for (size_t i = 0; i < m_suites.size(); ++i)
	{
		std::vector<size_t> dependencyMapping;

		for (size_t j = 0; j < m_suites[i]->GetDependencyCount(); ++j)
		{
			auto it = m_names.find(m_suites[i]->GetDependency(j));

			if (it != m_names.end())
			{
				size_t idx = it->second;
				dependencyMapping.push_back(idx);
			}
			else
			{
				// Dependency unknown error
			}
		}

		dependencies.push_back(std::move(dependencyMapping));
	}

	return TopologicalSortDependencies(_sorted, dependencies);
}

bool TestEnvironment::TopologicalSortDependencies(
	std::vector<TestSuite*>& _sorted, std::vector<std::vector<size_t>> const& _dependencies)
{
	_sorted.clear();

	std::vector<bool> tempmarked;
	tempmarked.resize(m_suites.size(), false);
	std::vector<bool> marked;
	marked.resize(m_suites.size(), false);

	for (size_t i = 0; i < m_suites.size(); ++i)
	{
		if (marked[i])
		{
			continue;
		}

		bool succeeded = TopologicalVisit(_sorted, _dependencies, i, tempmarked, marked);

		if (!succeeded)
		{
			_sorted.clear();

			return false;
		}
	}

	return true;
}

bool TestEnvironment::TopologicalVisit(
	std::vector<TestSuite*>& _sorted, std::vector<std::vector<size_t>> const& _dependencies, 
	size_t _idx, std::vector<bool>& _tempmarked, std::vector<bool>& _marked)
{
	if (_tempmarked[_idx])
	{
		return false;
	}

	if (!_marked[_idx])
	{
		_tempmarked[_idx] = true;

		for (size_t i = 0; i < _dependencies[_idx].size(); ++i)
		{
			TopologicalVisit(_sorted, _dependencies, _dependencies[_idx][i], _tempmarked, _marked);
		}

		_marked[_idx] = true;
		_tempmarked[_idx] = false;

		_sorted.push_back(m_suites[_idx]);
	}

	return true;
}