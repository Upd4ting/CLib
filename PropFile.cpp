#include "PropFile.h"

PropFile::PropFile(std::string n, int m, char s) : name(n), method(m), separator(s)
{
	if (m == LOAD_ALL)
		this->load("", true);
}

PropFile::PropFile(const PropFile& prop) : name(prop.getName()), method(prop.getMethod()), separator(prop.getSeparator())
{
	if (prop.getMethod() == LOAD_ALL)
		this->load("", true);
}

PropFile::~PropFile()
{
	if (this->file)
		this->file.close();
}

bool PropFile::load(std::string k, bool loadall)
{
	if (!this->file.is_open()) 
	{
		this->file.open(this->name.c_str());

		if (!this->file)
			return false;
	}

	// Rewind
	this->file.clear();
	this->file.seekg(0);

	// Read file and load key/value

	std::string line, key, value;

	while (std::getline(this->file, line)) 
	{
		std::stringstream  lineStream(line);

		if (!std::getline(lineStream, key, this->separator) || !std::getline(lineStream, value, this->separator))
		{
			return false; // Fichier pas formatté correctement donc load fail
		}

		if (loadall || key == k)
		{
			this->map[key] = value;
		}

		if (!loadall)
		{
			break;
		}
	}

	return true;
}

std::string& PropFile::operator[](std::string k)
{
	if (this->map.count(k))
		return this->map[k];

	if (this->method == LOAD_ON_GET)
	{
		bool loaded = this->load(k);

		if (loaded)
			return this->map[k];
	}

	static std::string s = ""; // Car on doit retourné une référence return "" n'est pas une référence

	return s;
}

std::string PropFile::getName() const
{
	return this->name;
}

char PropFile::getMethod() const
{
	return this->method;
}

char PropFile::getSeparator() const
{
	return this->separator;
}