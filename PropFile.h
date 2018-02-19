#pragma once

#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <utility>
#include <algorithm>
#include <string>

#define LOAD_ALL 0
#define LOAD_ON_GET 1

class PropFile
{
private:
	std::string name;
	std::map<std::string, std::string> map;
	std::ifstream file;
	char method;
	char separator;

	bool load(std::string k, bool loadall = false); // Load une key/value de la config, ou toute en fonction du bool
public:
	PropFile(std::string n, int m = LOAD_ALL, char s = '=');
	PropFile(const PropFile& prop);
	~PropFile();

	std::string& operator[](std::string k);

	std::string getName() const;
	char getMethod() const;
	char getSeparator() const;
};