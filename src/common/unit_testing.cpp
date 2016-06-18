#include "unit_testing.h"

namespace SylDev { namespace Common {

TestEnvironment* TestEnvironment::Instance = nullptr;

namespace TestData {

TestInfo::TestInfo(std::string _file, int32_t _line)
	: m_file(_file), m_line(_line)
{
}

AssertResult::AssertResult(TestInfo const& _info, std::string _message, bool _result, bool _internal)
	: m_info(_info), m_message(_message), m_result(_result), m_internal(_internal)
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

std::string TestResult::GetName() const
{
	return m_name;
}

std::vector<AssertResult> const& TestResult::GetResults() const
{
	return m_assertResults;
}

bool TestResult::GetTotalResult() const
{
	return m_totalResult;
}

SuiteResult::SuiteResult(std::string _name)
	: m_name(_name), m_totalResult(true)
{
}

void SuiteResult::Clear()
{
	m_testResults.clear();
	m_totalResult = true;
}

void SuiteResult::AddResult(TestResult const& _testResult)
{
	m_testResults.push_back(_testResult);

	if (!_testResult.GetTotalResult())
	{
		m_totalResult = false;
	}
}

std::string SuiteResult::GetName() const
{
	return m_name;
}

std::vector<TestResult> const& SuiteResult::GetResults() const
{
	return m_testResults;
}

bool SuiteResult::GetTotalResult() const
{
	return m_totalResult;
}

} // TestData

TestContext::TestContext(TestData::TestResult& _testResult)
	: m_testResult(_testResult)
{
}

void TestContext::AddResult(TestData::TestInfo const& _info, std::string _message, bool _succeeded)
{
	m_testResult.AddResult(TestData::AssertResult(_info, _message, _succeeded));
}

namespace Impl {

UnitTest::UnitTest(TestSuite& _suite, std::string _name, std::function<void(TestContext&)> _func)
	: m_name(_name), m_func(std::move(_func))
{
	_suite.RegisterUnitTest(this);
}

std::string UnitTest::GetName() const
{
	return m_name;
}

void UnitTest::Run(TestData::TestResult& _testResult)
{
	TestContext ctx(_testResult);
	m_func(ctx);
}

TestDependency::TestDependency(TestData::TestInfo const& _info, std::string _name)
	: m_info(_info), m_name(_name)
{
}

TestSuite::TestSuite(TestEnvironment& _environment, std::string _name)
	: m_name(_name)
{
	_environment.RegisterTestSuite(_name, this);
}

void TestSuite::RegisterDependency(TestData::TestInfo const& _info, std::string _dependency)
{
	m_dependencies.push_back(TestDependency(_info, _dependency));
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

TestDependency TestSuite::GetDependency(size_t _idx) const
{
	return m_dependencies[_idx];
}

size_t TestSuite::GetDependencyCount() const
{
	return m_dependencies.size();
}

void TestSuite::Run(TestData::SuiteResult& _suiteResult)
{
	if (m_init)
	{
		m_init();
	}

	for (auto ii = m_tests.begin(); ii != m_tests.end(); ++ii)
	{
		UnitTest* test = *ii;

		if (m_fixtureEnter)
		{
			m_fixtureEnter();
		}

		TestData::TestResult testResult(test->GetName());
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

RegisterDependency::RegisterDependency(TestSuite& _suite, TestData::TestInfo const& _info, std::string _dependency)
{
	_suite.RegisterDependency(_info, _dependency);
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

} // Impl

TestEnvironment& TestEnvironment::GetInstance()
{
	if (!Instance)
	{
		Instance = new TestEnvironment();
	}

	return *Instance;
}

TestEnvironment::TestEnvironment()
	: m_printer(nullptr)
{
}

void TestEnvironment::AssignPrinter(TestPrinter* _printer)
{
	m_printer = _printer;
}

bool TestEnvironment::Run()
{
	for (auto ii = m_suiteResults.begin(); ii != m_suiteResults.end(); ++ii)
	{
		TestData::SuiteResult& suiteResult = *ii;
		suiteResult.Clear();
	}

	std::vector<Impl::TestSuite*> sorted_suites;
	bool succeeded = SolveDependencies(sorted_suites);

	if (succeeded)
	{
		for (auto ii = sorted_suites.begin(); ii != sorted_suites.end(); ++ii)
		{
			Impl::TestSuite* suite = *ii;

			std::vector<Impl::TestDependency> failedDependencies;

			for (size_t i = 0; i < suite->GetDependencyCount(); ++i)
			{
				auto jj = m_names.find(suite->GetDependency(i).m_name);

				if (jj != m_names.end())
				{
					size_t idx = jj->second;

					if (!m_suiteResults[idx].GetTotalResult())
					{
						failedDependencies.push_back(suite->GetDependency(i));
					}
				}
			}

			auto jj = m_names.find(suite->GetName());

			if (jj != m_names.end())
			{
				size_t idx = jj->second;

				if (!failedDependencies.empty())
				{
					for (auto kk = failedDependencies.begin(); kk != failedDependencies.end(); ++kk)
					{
						Impl::TestDependency dependency = *kk;

						TestData::AssertResult assertResult(dependency.m_info, "Dependency " + dependency.m_name + " has failed to complete successfully.", false, true);
				
						TestData::TestResult testResult("dependency_failed");
						testResult.AddResult(assertResult);

						m_suiteResults[idx].AddResult(testResult);
					}
				}
				else
				{
					suite->Run(m_suiteResults[idx]);
				}

				if (!m_suiteResults[idx].GetTotalResult())
				{
					succeeded = false;
				}
			}
		}
	}

	if (m_printer)
	{
		m_printer->OnEnd(m_suiteResults);
	}

	return succeeded;
}

void TestEnvironment::RegisterTestSuite(std::string _name, Impl::TestSuite* _suite)
{
	m_names.emplace(_name, m_suites.size());
	m_suites.push_back(_suite);
	m_suiteResults.push_back(TestData::SuiteResult(_name));
}

bool TestEnvironment::SolveDependencies(std::vector<Impl::TestSuite*>& _sorted)
{
	std::vector<std::vector<std::pair<size_t, size_t>>> dependencies;

	for (size_t i = 0; i < m_suites.size(); ++i)
	{
		std::vector<std::pair<size_t, size_t>> dependencyMapping;

		for (size_t j = 0; j < m_suites[i]->GetDependencyCount(); ++j)
		{
			auto ii = m_names.find(m_suites[i]->GetDependency(j).m_name);

			if (ii != m_names.end())
			{
				size_t idx = ii->second;
				dependencyMapping.push_back(std::make_pair(j, idx));
			}
			else
			{
				TestData::AssertResult assertResult(m_suites[i]->GetDependency(j).m_info, "Dependency " + m_suites[i]->GetDependency(j).m_name + " unknown.", true, true);
				
				TestData::TestResult testResult("dependency_unknown");
				testResult.AddResult(assertResult);

				m_suiteResults[i].AddResult(testResult);
			}
		}

		dependencies.push_back(std::move(dependencyMapping));
	}

	return TopologicalSortDependencies(_sorted, dependencies);
}

bool TestEnvironment::TopologicalSortDependencies(
	std::vector<Impl::TestSuite*>& _sorted, std::vector<std::vector<std::pair<size_t, size_t>>> const& _dependencies)
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
	std::vector<Impl::TestSuite*>& _sorted, std::vector<std::vector<std::pair<size_t, size_t>>> const& _dependencies, 
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
			size_t suiteIdx = _dependencies[_idx][i].second;

			bool succeeded = TopologicalVisit(_sorted, _dependencies, suiteIdx, _tempmarked, _marked);

			if (!succeeded)
			{
				size_t dependencyIdx = _dependencies[_idx][i].first;
				Impl::TestDependency dependency = m_suites[_idx]->GetDependency(dependencyIdx);

				TestData::AssertResult assertResult(dependency.m_info, "Dependency " + dependency.m_name + " is unresolvable.", false, true);
				
				TestData::TestResult testResult("dependency_unresolvable");
				testResult.AddResult(assertResult);

				m_suiteResults[_idx].AddResult(testResult);

				return false;
			}
		}

		_marked[_idx] = true;
		_tempmarked[_idx] = false;

		_sorted.push_back(m_suites[_idx]);
	}

	return true;
}

} } // SylDev, Common