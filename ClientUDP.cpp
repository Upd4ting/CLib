#include "ClientUDP.h"

ClientUDP::ClientUDP(std::string i, int p, Logger *l, IPType t) : Client(i, p, l, t)
{
	this->socket = this->buildSocket();
}

ClientUDP::ClientUDP(const ClientUDP &cli) : Client(cli)
{
}

Socket* ClientUDP::buildSocket()
{
	SocketUDP *socket = new SocketUDP(this->logger);
	socket->init(this->ip, this->port, this->type);

	if (socket->getHSocket() == -1)
	{
		delete socket;
		return NULL;
	}

	return socket;
}

ClientUDP* ClientUDP::create(std::string i, int p, Logger *l, IPType t)
{
	ClientUDP *cli = new ClientUDP(i, p, l, t);

	if (cli->socket == NULL || !cli->socket->isConnected())
	{
		delete cli;
		return NULL;
	}
	
	return cli;
}