#pragma once

#include "Socket.h"

class Client
{
protected:
	std::string ip;
	int port;
	Logger *logger;
	IPType type;

	Socket* socket;

	// Constructeur par défaut
	Client(std::string i, int p, Logger *l, IPType t = IPType::UNICAST);

	// Constructeur de copie
	Client(const Client& cli);

	// Arrêter la connexion avec un message d'erreur
	bool stop(std::string message);
public:
	
	// Destructeur
	virtual ~Client();

	// Start le client
	void start();

	// Stop le client
	bool stop(bool eoc = true);

	// Getter & Setter

	Socket* getSocket() const;
	std::string getIP() const;
	int getPort() const;
};