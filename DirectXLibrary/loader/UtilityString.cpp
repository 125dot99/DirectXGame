#include "UtilityString.h"

#include <Windows.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <bitset>

wchar_t* gamelib::utility_string::StrToWchar(const std::string& name)
{
	const unsigned char WC = 128;
	wchar_t* wfilepath = new wchar_t[WC];
	MultiByteToWideChar(CP_ACP, 0, name.c_str(), -1, wfilepath, WC);
	return wfilepath;
}

bool gamelib::utility_string::CheckStr(const std::string& fullPath, const std::string& name)
{
	std::istringstream line_stream(fullPath);
	std::string key, prekey = fullPath;
	while (key != prekey)
	{
		prekey = key;
		std::getline(line_stream, key, '/');
		if (key == name)
		{
			return true;
		}
	}
	return false;
}

std::string gamelib::utility_string::ExtractFileName(const std::string& path)
{
	size_t pos1;
	//‹æØ‚è•¶š''‚ªo‚Ä‚­‚éˆê”ÔÅŒã‚Ì•”•ª‚ğŒŸõ
	pos1 = path.rfind('\\');
	if (pos1 != std::string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}
	//‹æØ‚è•¶š'/'‚ªo‚Ä‚­‚éˆê”ÔÅŒã‚Ì•”•ª‚ğŒŸõ
	pos1 = path.rfind('/');
	if (pos1 != std::string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}
	return path;
}

std::string gamelib::utility_string::LoadTextFile(const std::string& textFileName)
{
	srand(12345);
	std::ifstream file;
	file.open(textFileName, std::ios::binary);
	if (file.fail())
	{
		return "";
	}
	std::string str;
	getline(file, str);
	for (int i = 0, end = (int)str.size(); i < end; i++)
	{
		str[i] = str.c_str()[i] - rand();
	}
	file.close();
	return str;
}

void gamelib::utility_string::SaveText(const std::string& textFileName, const std::string& data)
{
	srand(12345);
	std::vector<std::bitset<8>> byte(data.size());
	for (int i = 0, end = (int)data.size(); i < end; i++)
	{
		byte[i] = std::bitset<8>((uint64_t)data.c_str()[i] + rand());
	}
	std::ofstream file(textFileName, std::ios::binary);
	for (int i = 0, end = (int)byte.size(); i < end; i++)
	{
		file.write((char*)&byte[i], sizeof(char));
	}
	byte.clear();
}
