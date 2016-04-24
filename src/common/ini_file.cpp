#include "ini_file.h"

#include <Windows.h>

using namespace SylDev::Common;

IniFile::IniFile(std::string _file)
	: m_file(_file)
{
}

int32_t IniFile::ReadInt(std::string _section, std::string _key)
{
	return GetPrivateProfileInt(_section.c_str(), _key.c_str(), 0, m_file.c_str());
}

void IniFile::WriteInt(std::string _section, std::string _key, int32_t _value)
{
	char Buffer[256];
	_itoa_s(_value, Buffer, 256, 10);

	WriteString(_section, _key, Buffer);
}

float IniFile::ReadFloat(std::string _section, std::string _key)
{
	std::string String = ReadString(_section, _key);

	return static_cast<float>(atof(String.c_str()));
}

void IniFile::WriteFloat(std::string _section, std::string _key, float _value)
{
	char Buffer[256];
	sprintf_s(Buffer, 256, "%f", _value);

	WriteString(_section, _key, Buffer);
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
	char buffer[256];
	GetPrivateProfileString(_section.c_str(), _key.c_str(), NULL, buffer, 256, m_file.c_str());

	return buffer;
}

void IniFile::WriteString(std::string _section, std::string _key, std::string _value)
{
	WritePrivateProfileString(_section.c_str(), _key.c_str(), _value.c_str(), m_file.c_str());
}