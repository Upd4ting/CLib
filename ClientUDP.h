#pragma once

#include "Client.h"
#include "SocketUDP.h"

class ClientUDP : public Client
{
private:
	// Constructeur par défaut
	ClientUDP(std::string i, int p, Logger *l, IPType t = IPType::UNICAST);

	// Constructeur de copie
	ClientUDP(const ClientUDP& cli);

	Socket* buildSocket();
public:
	static ClientUDP* create(std::string i, int p, Logger *l, IPType t = IPType::UNICAST);
};