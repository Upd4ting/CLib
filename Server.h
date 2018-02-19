#pragma once

#include "Socket.h"
#include "SocketTCP.h"
#include "SocketUDP.h"

class Server
{
protected:
	std::string ip;
	int port;
	bool running;
	Logger *logger;
	IPType type;

	Socket *socket;

	// Contructeur de base
	Server(std::string i, int p, Logger *l, IPType t = IPType::UNICAST);

	// Constructeur de copie
	Server(const Server& serv);

	// Stop le serveur avec message d'erreur !
	bool stop(std::string message);

public:
	// Destructeur
	virtual ~Server();

	// Lance le serveur
	void start();

	// Stop le serveur
	bool stop(bool eoc = true);

	// Fonction virtuelle pour les différentes implémentation
	// des différents serveurs (MultiThread, MonoThread, Pool de thread)
	virtual void onStart() = 0;
	virtual void onStop(bool eoc) = 0;

	// Getter & Setter

	Socket* getSocket() const;
	bool isRunning() const;
	std::string getIP() const;
	int getPort() const;
};