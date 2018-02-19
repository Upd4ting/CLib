#pragma once

#include "Client.h"
#include "SocketTCP.h"

class ClientTCP : public Client
{
private:
	// Constructeur par défaut
	ClientTCP(std::string i, int p, Logger *l, IPType t = IPType::UNICAST);

	// Constructeur de copie
	ClientTCP(const ClientTCP& cli);

	Socket* buildSocket();
public:
	static ClientTCP* create(std::string i, int p, Logger *l, IPType t = IPType::UNICAST);
};