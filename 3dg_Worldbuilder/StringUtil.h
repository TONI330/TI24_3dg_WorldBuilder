#pragma once

#include <string>
#include <vector>
#include <algorithm>
class StringUtil
{
public:
	/**
	* Returns the index of a string if this string is in the array, else it returns -1
	*/
	static int IndexOf(std::vector<std::string> params, const char* check);

	/**
	* Replaces a substring in a string
	*/
	static std::string replace(std::string str, const std::string& toReplace, const std::string& replacement);

	/**
	* Splits a string into substrings, based on a seperator
	*/
	static std::vector<std::string> split(std::string str, const std::string& seperator);

	/**
	* Turns a string to lowercase
	*/
	static std::string toLower(std::string data);


	/**
	* Cleans up a line for processing
	*/
	static std::string cleanLine(std::string line);



};

