#include "Server.h"

Server::Server(std::string i, int p, Logger *l, IPType t) : ip(i), port(p), running(false), logger(l), type(t)
{
}

Server::Server(const Server& serv)
{
}

Server::~Server()
{
	if (this->socket)
		delete this->socket;
}

void Server::start()
{
	if (this->running)
	{
		this->logger->write("Socket déjà lancée.");
		return;
	}

	this->running = true;

	this->onStart();
}

bool Server::stop(bool eoc)
{
	this->running = false;
	this->onStop(eoc);
	return this->socket->stop(false);
}

bool Server::stop(std::string message)
{
	if (!this->running) // Ne pas display d'erreur et essayer de fermer alors que l'erreur est du a la fermeture du serveur
		return false;

	this->logger->write("Arrêt du serveur à cause d'une erreur: " + message);

	return this->stop(false);
}

Socket* Server::getSocket() const
{
	return this->socket;
}

bool Server::isRunning() const
{
	return this->running;
}

std::string Server::getIP() const
{
	return this->ip;
}

int Server::getPort() const
{
	return this->port;
}