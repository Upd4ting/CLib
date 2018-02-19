#include "ClientTCP.h"

ClientTCP::ClientTCP(std::string i, int p, Logger *l, IPType t) : Client(i, p, l, t)
{
	this->socket = this->buildSocket();
}

ClientTCP::ClientTCP(const ClientTCP &cli) : Client(cli)
{
}

Socket* ClientTCP::buildSocket()
{
	SocketTCP *socket = new SocketTCP(this->logger);

	struct sockaddr_in address;
	unsigned int tailleAddress;

	socket->init(this->ip, this->port, this->type);

	if (socket->getHSocket() == -1)
	{
		delete socket;
		return NULL;
	}

	address = *(socket->getAddress());
	tailleAddress = sizeof(struct sockaddr_in);
	if (connect(socket->getHSocket(), (struct sockaddr *)&address, tailleAddress) == -1)
	{
		this->logger->write("Erreur sur connect de la socket: " + std::string(strerror(errno)));
		return NULL;
	}

	return socket;
}

ClientTCP* ClientTCP::create(std::string i, int p, Logger *l, IPType t)
{
	ClientTCP *cli = new ClientTCP(i, p, l, t);

	if (cli->socket == NULL || !cli->socket->isConnected())
	{
		delete cli;
		return NULL;
	}

	return cli;
}