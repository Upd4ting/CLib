#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <functional>
#include <utility>
#include <pthread.h>
#include <signal.h>
#include "Input.h"

typedef struct Item {
	std::string name;
	std::function<bool(int)> callback;
} ITEM;

typedef std::vector<ITEM> ITEMS;

void handlerSigusr1(int sig);
void* threadMenu(void *param);

class Menu
{
private:
	pthread_t thread;
	bool running;

	std::string name;
	ITEMS items;
	bool askToReturn;
public:

	friend void* threadMenu(void *param);

	Menu();

	// Constructeur par défaut
	Menu(std::string n);

	// Constructeur de copie
	Menu(const Menu& menu);

	// Destructeur
	~Menu();

	// Ajout d'une option dans le menu
	Menu& addItem(std::string n, std::function<bool(int)> c);

	// Lance le menu
	void run();

	// Stop le menu
	void stop();

	// Getter & Setter

	std::string getName() const;
	const ITEMS& getItems() const;

	void setAskToReturn(bool b);
};