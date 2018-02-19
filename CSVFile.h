#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <utility>
#include <algorithm>
#include <iostream>

typedef std::string CSVCell;
typedef std::vector<std::string> CSVHeaders;
typedef std::map<std::string, std::string> CSVLineInfo;
typedef std::pair<std::string, std::string> CSVCond;

class CSVFile
{
private:
	std::string name;
	bool open;
	std::ifstream file;
	char separator;

public:
	CSVFile(std::string n, char s = ';');
	CSVFile(const CSVFile &cf);
	~CSVFile();

	bool openFile(); // Ouvrir le fichier. Renvoie true/false si l'opération a réussi
	bool readLine(std::vector<CSVCell> &result); // Lis une ligne et renvoie true/false si la ligne a été lue ou pas, et met les info dans le vecteur passé en paramètre

	bool searchFirst(std::vector<CSVCond> conditions, CSVLineInfo *li); // Cherche la première ligne du CSV qui satisfait les conditions, headers dans le fichier
	bool searchFirst(CSVHeaders headers, std::vector<CSVCond> conditions, CSVLineInfo *li, bool needrewind = true); // Même qu'au dessus mais headers pas dans le fichier

	bool searchFirst(CSVCond condition, CSVLineInfo *li);
	bool searchFirst(CSVHeaders headers, CSVCond condition, CSVLineInfo *li);

	void rewind(); // Revenir au début du fichier

	std::string getName() const;
	bool isOpen() const;
	char getSeparator() const;
};