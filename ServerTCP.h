#pragma once

#include "Server.h"

typedef std::function<void(SocketTCP*)> SERVERLISTENER;
typedef std::vector<SERVERLISTENER> SERVERLISTENERS;

void* mainThread(void *param);

class ServerTCP : public Server
{
protected:
	SERVERLISTENERS listeners;
	pthread_t thread;

	// Initialisation d'une machine a état pour TCP
	bool initListen();

	// Constructeur de copie
	ServerTCP(const ServerTCP& serv);
public:
	friend void* mainThread(void *param);

	// Contructeur de base
	ServerTCP(std::string i, int p, Logger *l, IPType t = IPType::UNICAST);

	Socket* buildSocket();
	void onStart();
	void onStop(bool eoc);
	virtual void onClientAccept(SocketTCP *socket) = 0;

	void addListener(SERVERLISTENER listener);
};