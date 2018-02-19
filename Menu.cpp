#include "Menu.h"

void handlerSigusr1(int sig)
{
}

void* threadMenu(void *param)
{
	Menu *menu = (Menu*) param;

	while (menu->running)
	{
		system("clear");

		std::cout << menu->getName() << std::endl;

		for (unsigned int i = 0; i < menu->getName().length(); i++)
			std::cout << "=";
		std::cout << std::endl << std::endl;

		// Display menu
		for (ITEMS::iterator it = menu->items.begin(); it != menu->items.end(); it++)
		{
			ITEM item = *it;
			int num = it - menu->items.begin() + 1;
			std::cout << num << ". " << item.name << std::endl;
		}

		unsigned int choice;

		do
		{
			std::cout << "Votre choix => ";
			choice = readInt();

			if (!menu->running) // Le menu a été interrompu par SIGUSR1
				break;

			if (choice <= 0 || choice > menu->items.size())
				std::cout << "Votre choix est incorrecte !" << std::endl;
		} while (choice <= 0 || choice > menu->items.size());

		if (!menu->running) // Le menu a été interrompu par SIGUSR1
			break;

		std::cout << std::endl << std::endl;

		ITEM item = menu->items.at(choice - 1);

		bool stop = item.callback(choice - 1);

		if (stop)
			break;

		if (menu->askToReturn)
		{
			fflush(stdin);
			std::cout << "Entrer quelque chose pour revenir au menu..." << std::endl;
			std::cin.get();
		}
		else
			menu->askToReturn = true;
	}

	return NULL;
}

Menu::Menu()
{
	
}

Menu::Menu(std::string n) : name(n), askToReturn(true)
{

}

Menu::Menu(const Menu& menu) : name(menu.getName())
{
	this->items = ITEMS(menu.getItems().begin(), menu.getItems().end());
}

Menu::~Menu()
{
}

Menu& Menu::addItem(std::string n, std::function<bool(int)> c)
{
	ITEM item = { .name = n, .callback = c};
	this->items.push_back(item);
	return *this;
}

void Menu::run()
{
	struct sigaction act;
	act.sa_handler = handlerSigusr1;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGUSR1, &act, 0);

	this->running = true;
	pthread_create(&(this->thread), NULL, threadMenu, (void*)this);
	pthread_join(this->thread, NULL);
}

void Menu::stop()
{
	this->running = false;
	pthread_kill(this->thread, SIGUSR1);
}

std::string Menu::getName() const 
{
	return this->name;
}

const ITEMS& Menu::getItems() const
{
	return this->items;
}

void Menu::setAskToReturn(bool b)
{
	this->askToReturn = b;
}