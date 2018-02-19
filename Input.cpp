#include "Input.h"

static const char alphanum[] =
"0123456789"
"!@#$%^&*"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";

std::string getRandomString()
{
	std::string s = "";

	for (int i = 0; i < 9; i ++)
		s = s + alphanum[rand() % strlen(alphanum)];

	return s;
}

int convertInt(std::string s)
{
	std::istringstream buffer(s);
	int value;
	buffer >> value;
	return value;
}

double convertDouble(std::string s)
{
	std::istringstream buffer(s);
	double value;
	buffer >> value;
	return value;
}

std::string convertString(int number)
{
	std::stringstream convert;
	convert << number;
	return convert.str();
}

std::string convertString(double number)
{
	std::stringstream convert;
	convert << number;
	return convert.str();
}


std::vector<std::string> splitString(std::string s, std::string delimiter)
{
	std::vector<std::string> list;

	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
	    token = s.substr(0, pos);
	    list.push_back(token);
	    s.erase(0, pos + delimiter.length());
	}
	list.push_back(s);

	return list;
}

std::vector<std::string> splitString(char *buffer, std::string delimiter)
{
	return splitString(std::string(buffer), delimiter);
}

int countZero(std::string s)
{
	return (int) std::count(s.begin(), s.end(), '0');
}

int readInt()
{
	int number;
	fflush(stdin);
	std::cin.clear();

	std::string str;
	std::getline(std::cin, str);

	try
	{
		number = std::stoi(str);
	}
	catch (std::exception& e)
	{
		number = -1;
	}

	return number;
}

double readDouble()
{
	double number;
	fflush(stdin);
	std::cin.clear();

	std::string str;
	std::getline(std::cin, str);

	try
	{
		number = std::stod(str);
	}
	catch (std::exception& e)
	{
		number = -1;
	}

	return number;
}

void readString(char *buffer)
{
	fflush(stdin);
	std::cin.clear();

	std::string str;
	std::getline(std::cin, str);
	std::strcpy(buffer, str.c_str());
}