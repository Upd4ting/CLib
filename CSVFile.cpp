#include "CSVFile.h"

CSVFile::CSVFile(std::string n, char s) : name(n), open(false), separator(s)
{
}

CSVFile::CSVFile(const CSVFile& cf) : name(cf.getName()), open(false), separator(cf.getSeparator())
{
}

CSVFile::~CSVFile()
{
	if (this->file)
		this->file.close();
}

bool CSVFile::openFile()
{
	if (this->isOpen())
		return true;

	this->file.open(this->getName().c_str());
	this->open = this->file.is_open();
	return this->open;
}

bool CSVFile::readLine(std::vector<CSVCell> &result)
{
	if (!this->isOpen())
	{
		this->openFile();

		if (!this->isOpen())
		{
			return false;
		}
	}

	// Clear vector
	result.clear();

	std::string line;

	if (std::getline(this->file, line)) // Ligne a été lue correctement
	{
		std::stringstream  lineStream(line);
		CSVCell            cell;

		while (std::getline(lineStream, cell, this->separator))
			result.push_back(cell);
	}
	else
	{
		return false;
	}

	return true;
}

bool CSVFile::searchFirst(std::vector<CSVCond> conditions, CSVLineInfo *li)
{
	// Read headers in file
	this->rewind();

	CSVHeaders headers;
	bool success = this->readLine(headers);

	if (!success)
	{
		return false;
	}

	return this->searchFirst(headers, conditions, li, false);
}

bool CSVFile::searchFirst(CSVHeaders headers, std::vector<CSVCond> conditions, CSVLineInfo *li, bool needrewind)
{
	if (needrewind)
	{
		this->rewind();
	}

	std::vector<CSVCell> cells;
	bool founded = false;

	while (this->readLine(cells))
	{
		bool respect = true;

		// On check chaque condition
		for (std::vector<CSVCond>::iterator it = conditions.begin(); it != conditions.end() && respect; it++)
		{
			CSVCond cond = *it;
			int pos = std::find(headers.begin(), headers.end(), cond.first) - headers.begin();

			if (cells.at(pos) != cond.second)
				respect = false;
		}

		if (respect) // La ligne respecte chacune de nos conditions
		{
			// Map each header to his cell
			for (CSVHeaders::iterator it = headers.begin(); it != headers.end(); it++)
			{
				std::string head = *it;

				(*li)[head] = cells.at(it - headers.begin());
			}

			founded = true;

			break;
		}
	}

	return founded;
}

bool CSVFile::searchFirst(CSVCond condition, CSVLineInfo *li)
{
	std::vector<CSVCond> vector(1, condition);
	return this->searchFirst(vector, li);
}

bool CSVFile::searchFirst(CSVHeaders headers, CSVCond condition, CSVLineInfo *li)
{
	std::vector<CSVCond> vector(1, condition);
	return this->searchFirst(headers, vector, li);
}

void CSVFile::rewind() 
{
	this->file.clear();
	this->file.seekg(0);
}

std::string CSVFile::getName() const {
	return this->name;
}

bool CSVFile::isOpen() const {
	return this->open;
}

char CSVFile::getSeparator() const {
	return this->separator;
}