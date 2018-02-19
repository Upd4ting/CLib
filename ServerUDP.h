#pragma once

#include "Server.h"

class ServerUDP : public Server
{
private:
	// Contructeur de base
	ServerUDP(std::string i, int p, Logger *l, IPType t = IPType::UNICAST);

	// Constructeur de copie
	ServerUDP(const ServerUDP& serv);
public:

	Socket* buildSocket();
	void onStart();
	void onStop(bool eoc);

	static ServerUDP* create(std::string ip, int port, int mt, Logger *l, IPType t = IPType::UNICAST);
};