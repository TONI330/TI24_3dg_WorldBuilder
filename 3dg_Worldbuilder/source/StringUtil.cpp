#include "StringUtil.h"

int StringUtil::IndexOf(std::vector<std::string> params, const char* check)
{
	for (int i = 0; i < params.size(); i++)
	{
		if (params[i] == check)

			return i;
	}
	return -1;
}
std::string StringUtil::replace(std::string str, const std::string& toReplace, const std::string& replacement)
{
	size_t index = 0;
	while (true)
	{
		index = str.find(toReplace, index);
		if (index == std::string::npos)
			break;
		str.replace(index, toReplace.length(), replacement);
		++index;
	}
	return str;
}

std::vector<std::string> StringUtil::split(std::string str, const std::string& seperator)
{
	std::vector<std::string> ret;
	size_t index;
	while (true)
	{
		index = str.find(seperator);
		if (index == std::string::npos)
			break;
		ret.push_back(str.substr(0, index));
		str = str.substr(index + 1);
	}
	ret.push_back(str);
	return ret;
}

std::string StringUtil::toLower(std::string data)
{
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	return data;
}

std::string StringUtil::cleanLine(std::string line)
{
	line = replace(line, "\t", " ");
	while (line.find("  ") != std::string::npos)
		line = replace(line, "  ", " ");
	if (line == "")
		return "";
	if (line[0] == ' ')
		line = line.substr(1);
	if (line == "")
		return "";
	if (line[line.length() - 1] == ' ')
		line = line.substr(0, line.length() - 1);
	return line;
}