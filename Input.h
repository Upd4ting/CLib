#pragma once
#define _GLIBCXX_USE_C99 1

#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <cstring>
#include <exception>
#include <ctime>
#include <vector>

std::string getRandomString();
int convertInt(std::string s);
double convertDouble(std::string s);
std::string convertString(int number);
std::string convertString(double number);
std::vector<std::string> splitString(std::string s, std::string delimiter);
std::vector<std::string> splitString(char *buffer, std::string delimiter);
int countZero(std::string s);
int readInt();
void readString(char *buffer);
double readDouble();