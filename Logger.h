#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <ctime>

class Logger
{
private:
	std::string name;

	Logger(const Logger& log);
public:
	Logger(std::string n);
	~Logger();

	bool write(std::string message);
};