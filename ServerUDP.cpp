#include "ServerUDP.h"

ServerUDP::ServerUDP(const ServerUDP& serv) : Server(serv)
{
}

ServerUDP::ServerUDP(std::string i, int p, Logger *l, IPType t) : Server(i, p, l, t)
{
	this->socket = this->buildSocket();
}

Socket* ServerUDP::buildSocket()
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

void ServerUDP::onStart()
{
	this->socket->start();
}

void ServerUDP::onStop(bool eoc)
{
}

ServerUDP* ServerUDP::create(std::string ip, int port, int mt, Logger *l, IPType t)
{
	ServerUDP *serv = new ServerUDP(ip, port, l, t);

	if (serv->socket == NULL || !serv->socket->isConnected())
	{
		delete serv;
		return NULL;
	}

	return serv;
}