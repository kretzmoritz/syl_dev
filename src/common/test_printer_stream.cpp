#include "test_printer_stream.h"

#include <iostream>
#include <fstream>

namespace SylDev { namespace Common {

void TestPrinterStream::OnEnd(std::vector<TestData::SuiteResult> const& _suiteResults)
{
	m_stream.clear();

	for (auto ii = _suiteResults.begin(); ii != _suiteResults.end(); ++ii)
	{
		TestData::SuiteResult const& suiteResult = *ii;
		std::string suiteName = suiteResult.GetName();

		std::vector<TestData::TestResult> const& testResults = suiteResult.GetResults();

		if (!testResults.empty())
		{
			std::string output = "Suite: " + suiteName;

			m_stream << output << std::endl;

			for (size_t i = 0; i < output.length(); ++i)
			{
				m_stream << '-';
			}
			m_stream << std::endl;

			m_stream << std::endl;
		}

		for (auto jj = testResults.begin(); jj != testResults.end(); ++jj)
		{
			TestData::TestResult const& testResult = *jj;
			std::string testName = testResult.GetName();

			std::vector<TestData::AssertResult> const& assertResults = testResult.GetResults();

			for (auto kk = assertResults.begin(); kk != assertResults.end(); ++kk)
			{
				TestData::AssertResult const& assertResult = *kk;

				if (assertResult.m_internal)
				{
					if (assertResult.m_result)
					{
						m_stream << "Internal ";
					}
					else
					{
						m_stream << "Critical internal ";
					}

					m_stream << "error " + testName + "." << std::endl;
					m_stream << "File: " + assertResult.m_info.m_file + " Line: " + std::to_string(assertResult.m_info.m_line) << std::endl;
					m_stream << assertResult.m_message << std::endl;
					m_stream << std::endl;
				}
				else
				{
					m_stream << "Test " + testName + "." << std::endl;
					m_stream << "File: " + assertResult.m_info.m_file + " Line: " + std::to_string(assertResult.m_info.m_line) << std::endl;
					m_stream << "Assert " << assertResult.m_message << " has " << (assertResult.m_result ? "succeeded" : "failed") << '.' << std::endl;
					m_stream << std::endl;
				}
			}
		}
	}
}

void TestPrinterStream::OutputToConsole()
{
	std::cout << m_stream.str();
}

void TestPrinterStream::OutputToFile(std::string _file)
{
	std::fstream fstream;
	fstream.open(_file, std::ios::out);

	if (!fstream.is_open())
	{
		return;
	}

	fstream << m_stream.str();

	fstream.close();
}

} } // SylDev, Common