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

TestSuite::TestSuite(TestEnvironment& _environment, std::string _name)
{
	_environment.RegisterTestSuite(_name, this);
}

std::string TestSuite::GetDependency(size_t _idx) const
{
	return m_dependencies[_idx];
}

size_t TestSuite::GetDependencyCount() const
{
	return m_dependencies.size();
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

TestResult TestSuite::Run()
{
	TestResult totalResult = TestResult::Succeeded;

	if (m_init)
	{
		m_init();
	}

	for (auto it = m_tests.begin(); it != m_tests.end(); ++it)
	{
		if (m_fixtureEnter)
		{
			m_fixtureEnter();
		}

		TestResult result = (*it)->Run();

		if (m_fixtureLeave)
		{
			m_fixtureLeave();
		}

		if (totalResult == TestResult::Failed)
		{
			continue;
		}

		if (result == TestResult::Failed || result == TestResult::Unknown)
		{
			totalResult = result;
		}
	}

	if (m_close)
	{
		m_close();
	}

	return totalResult;
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

TestResult TestEnvironment::Run()
{
	TestResult totalResult = TestResult::Succeeded;

	std::vector<TestSuite*> suites;
	bool succeeded = SolveDependencies(suites);

	if(!succeeded)
	{
		// To do: resolve error

		return TestResult::Failed;
	}

	for (size_t i = 0; i < suites.size(); ++i)
	{
		bool DependencyError = false;

		for (size_t j = 0; j < suites[i]->GetDependencyCount(); ++j)
		{
			auto it = m_names.find(suites[i]->GetDependency(j));

			if (it != m_names.end())
			{
				if (m_results[it->second] != TestResult::Succeeded)
				{
					// To do: test we depend on has failed

					DependencyError = true;
				}
			}
		}

		if (DependencyError)
		{
			m_results[i] = TestResult::Failed;
		}
		else
		{
			m_results[i] = suites[i]->Run();
		}

		if (totalResult == TestResult::Failed)
		{
			continue;
		}

		if (m_results[i] == TestResult::Failed || m_results[i] == TestResult::Unknown)
		{
			totalResult = m_results[i];
		}
	}

	return totalResult;
}

void TestEnvironment::RegisterTestSuite(std::string _name, TestSuite* _suite)
{
	m_names.emplace(_name, m_suites.size());
	m_suites.push_back(_suite);
	m_results.push_back(TestResult::Unknown);
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
				dependencyMapping.push_back(it->second);
			}
			else
			{
				// To do: dependency unknown error
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

		bool succeeded = Visit(_sorted, _dependencies, i, tempmarked, marked);

		if (!succeeded)
		{
			_sorted.clear();

			return false;
		}
	}

	return true;
}

bool TestEnvironment::Visit(
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
			Visit(_sorted, _dependencies, _dependencies[_idx][i], _tempmarked, _marked);
		}

		_marked[_idx] = true;
		_tempmarked[_idx] = false;

		_sorted.push_back(m_suites[_idx]);
	}

	return true;
}