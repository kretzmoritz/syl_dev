#include "ini_file.h"

#include <Windows.h>

namespace SylDev { namespace Common {

IniFile::IniFile(std::string _file)
	: m_file(_file)
{
}

int32_t IniFile::ReadInt(std::string _section, std::string _key)
{
	return GetPrivateProfileInt(nowide::widen(_section).c_str(), nowide::widen(_key).c_str(), 0, nowide::widen(m_file).c_str());
}

void IniFile::WriteInt(std::string _section, std::string _key, int32_t _value)
{
	char buffer[256];
	_itoa_s(_value, buffer, 256, 10);

	WriteString(_section, _key, buffer);
}

float IniFile::ReadFloat(std::string _section, std::string _key)
{
	std::string string = ReadString(_section, _key);

	return static_cast<float>(atof(string.c_str()));
}

void IniFile::WriteFloat(std::string _section, std::string _key, float _value)
{
	char buffer[256];
	sprintf_s(buffer, 256, "%f", _value);

	WriteString(_section, _key, buffer);
}

bool IniFile::ReadBool(std::string _section, std::string _key)
{
	return ReadInt(_section, _key) != 0;
}

void IniFile::WriteBool(std::string _section, std::string _key, bool _value)
{
	WriteInt(_section, _key, _value);
}

std::string IniFile::ReadString(std::string _section, std::string _key)
{
	wchar_t buffer[256];
	GetPrivateProfileString(nowide::widen(_section).c_str(), nowide::widen(_key).c_str(), NULL, buffer, 256, nowide::widen(m_file).c_str());

	return nowide::narrow(buffer);
}

void IniFile::WriteString(std::string _section, std::string _key, std::string _value)
{
	std::experimental::filesystem::path directory = std::experimental::filesystem::u8path(m_file);
	directory.remove_filename();

	if (!std::experimental::filesystem::exists(directory))
	{
		std::experimental::filesystem::create_directory(directory);
	}

	WritePrivateProfileString(nowide::widen(_section).c_str(), nowide::widen(_key).c_str(), nowide::widen(_value).c_str(), nowide::widen(m_file).c_str());
}

} } // SylDev, Common