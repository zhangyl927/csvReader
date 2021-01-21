#ifndef __TOOL_H
#define __TOOL_H

#include <iostream>
#include <cmath>

namespace readCSV
{

void sys_error(const char* str)
{
    std::cout << str << std::endl;
    exit(-1);
}

// string to int
uint64_t strToInt(std::string& str)
{
	int i = 0, point = 0, num = 0;
	uint64_t res = 0;
	for (; i < str.size(); ++i)
	{
		if ('.' == str[i]) point = i;
		if ('E' == str[i]) break;
	}
	if (i == str.size()) return atoi(str.c_str());
	
	for (int j = i + 2; j < str.size(); ++j)
	{
		num = num*10 + str[j] - '0';
	}

	int idx = 0;
	for (int k = 0; k < i; ++k)
	{
		if (point < k) idx = point - k;
		else if (point == k) continue;
		else idx = point-k-1;
		res = res + (str[k] - '0') * pow(10, num+idx);
	}

	return res;
}

// string to double
double strToDouble(const std::string& str)
{
	int point = 0, idx = 0;
	for (; point < str.size(); ++point)
	{
		if ('.' == str[point]) break;
	}

	double res = 0;
	for (int k = 0; k < str.size(); ++k)
	{
		if (point < k) idx = point - k;
		else if (point == k) continue;
		else idx = point - k - 1;
		res = res + (str[k] - '0') * pow(10, idx);
	}

	return res;
}

}

#endif