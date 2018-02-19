#pragma once

#include "Socket.h"

void* cThreadFunctionA(void *param);

class SocketTCP : public Socket
{
private:
	pthread_t thread;

	int ssend(void *data, size_t size, int flag = 0);
	int rrecv(void *data, size_t size, int flag = 0);

	SocketTCP(const SocketTCP& so);
public:
	friend void* cThreadFunctionA(void *param);

	SocketTCP(Logger *l);
	SocketTCP(int hS, struct sockaddr_in a, Logger *l);
	
	void init(std::string data, int port, IPType type);
	void link();

	void onStart(bool wait);
	void onStop(bool eoc);

	// Ecrit des données sur le buffer
	// Retourne le nombre de byte écrit
	int write(void *data, size_t size, bool writeSize = false, int flag = 0);

	// Lire une certaine taille de données depuis le buffer
	int read(void *data, size_t size, int flag = 0);

	// Lire des données dont la taille 
	// a été envoyée via un int sur le buffer
	int read(void *data, int flag = 0);

	// Lire des données sur le buffer jusqu'a un 
	// certain caractère qui sépare les trames.
	int read(void *data, char separator, int flag = 0);
};