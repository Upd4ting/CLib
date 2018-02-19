#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h> /* pour exit */
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h> 
#include <errno.h>
#include <netinet/in.h> 
#include <netinet/tcp.h> 
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <functional>
#include <vector>
#include <utility>
#include "Logger.h"

#define END_OF_CONNECTION "END_OF_CONNECTION"
#define DENY_OF_CONNECTION "DENY_OF_CONNECTION"
#define CLOSE_END_CONNECTION 0
#define CLOSE_DENY_CONNECTION 1
#define CLOSE_ERROR_CONNECTION 2

enum IPType { UNICAST, MULTICAST };

class Socket;

typedef std::function<void(int)> CLOSELISTENER;
typedef std::function<void(Socket*)> MESSAGELISTENER;

class Socket
{
protected:  
	int hSocket;
	struct sockaddr_in address;
	Logger *logger;

	CLOSELISTENER clistener;
	MESSAGELISTENER mlistener;

	bool running;

	// Fermer le socket avec un message d'erreur
	// Retourne true si pas d'erreur
	bool stop(std::string message);

	// Constructeur par défaut
	Socket(Logger *l);

	// Constructeur avec paramètre
	// Pour initiliaser un socket qui est déjà créer 
	// On a juste besoin d'encapsuler ses informations
	Socket(int hS, struct sockaddr_in a, Logger *l);

	// Constructeur de copie
	Socket(const Socket& so);
public:
	// Destructeur
	virtual ~Socket();

	// Initialiser la socket
	virtual void init(std::string data, int port, IPType type) = 0;

	virtual void onStart(bool wait) = 0;
	virtual void onStop(bool eoc) = 0;

	// On démarre la socket
	void start(bool wait = false);

	// Fermer la socket
	// Retourne true si pas d'erreur
	bool stop(bool eoc = true);

	// Ajoute un message listener
	void setMessageListener(MESSAGELISTENER listener);

	// Ajoute un close listener
	void setCloseListener(CLOSELISTENER listener);

	// Getter & Setter

	int getHSocket() const;
	const struct sockaddr_in* getAddress() const;
	bool isConnected() const;
};