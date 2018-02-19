#include "Client.h"

Client::Client(std::string i, int p, Logger *l, IPType t) : ip(i), port(p), logger(l), type(t)
{
}

Client::Client(const Client &cli)
{
}

Client::~Client()
{
	if (this->socket)
		delete this->socket;
}

void Client::start()
{
	this->socket->start();
}

bool Client::stop(bool eoc)
{
	return this->socket->stop(eoc);
}

bool Client::stop(std::string message)
{
	this->logger->write("Arrêt du client à cause d'une erreur: " + message);

	return this->stop(false);
}

Socket* Client::getSocket() const
{
	return this->socket;
}

std::string Client::getIP() const
{
	return this->ip;
}

int Client::getPort() const
{
	return this->port;
}