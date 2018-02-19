#include "Logger.h"

Logger::Logger(std::string n) : name(n)
{
}

Logger::Logger(const Logger& log)
{
}

Logger::~Logger()
{
}

bool Logger::write(std::string message)
{
	std::ofstream file;
	file.open(this->name.c_str(), std::ios::app);

	if (!file.is_open())
		return false;

	// GET DATE
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time (&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer,sizeof(buffer),"%d-%m-%Y %I:%M:%S",timeinfo);
	std::string date(buffer);

	// Write date before message
	file << date << " > " << message << std::endl;

	std::cout << date << " > " << message << std::endl;

	bool good = file.good();

	file.close();

	return good;
}