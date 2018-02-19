#include "ServerTCP.h"

void* mainThread(void *param)
{
	ServerTCP *serv = (ServerTCP*)param;
	struct sockaddr_in address;
	unsigned int tailleAddress;
	int hSocketService;

	bool success = serv->initListen();

	if (!success)
		return NULL;

	while (serv->running)
	{
		address = *(serv->socket->getAddress());
		tailleAddress = sizeof(struct sockaddr_in);
		hSocketService = -1;

		if ((hSocketService = accept(serv->socket->getHSocket(), (struct sockaddr*)&address, &tailleAddress)) == -1)
		{
			serv->stop("Erreur sur l'accept de la socket: " + std::string(strerror(errno)));
			return NULL;
		}

		SocketTCP *socket = new SocketTCP(hSocketService, address, serv->logger);

		for (SERVERLISTENERS::iterator it = serv->listeners.begin(); it != serv->listeners.end(); it++)
		{
			(*it)(socket);
		}

		serv->onClientAccept(socket);

		serv->logger->write("Nouveau client connecté !");
	}

	return NULL;
}

bool ServerTCP::initListen()
{
	if (listen(this->socket->getHSocket(), 32) == -1)
	{
		this->logger->write("Erreur sur le listen de la socket: " + std::string(strerror(errno)));

		this->stop();

		return false;
	}

	return true;
}

ServerTCP::ServerTCP(const ServerTCP& serv) : Server(serv)
{
}

ServerTCP::ServerTCP(std::string i, int p, Logger *l, IPType t) : Server(i, p, l, t)
{
	this->socket = this->buildSocket();
}

Socket* ServerTCP::buildSocket()
{
	SocketTCP *socket = new SocketTCP(this->logger);

	socket->init(this->ip, this->port, this->type);

	if (socket->getHSocket() == -1)
	{
		delete socket;
		return NULL;
	}

	socket->link();

	return socket;
}

void ServerTCP::onStart()
{
	// Lancement du thread
	pthread_create(&(this->thread), NULL, mainThread, (void*)this);
}

void ServerTCP::onStop(bool eoc)
{
}

void ServerTCP::addListener(SERVERLISTENER listener)
{
	this->listeners.push_back(listener);
}