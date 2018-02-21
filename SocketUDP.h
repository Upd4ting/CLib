#pragma once

#include "Socket.h"
#include <sys/time.h>

void* cThreadFunctionB(void *param);

class SocketUDP : public Socket
{
private:
	struct sockaddr_in from;
	pthread_t thread;
	int bufsize;
	char *packet;

	int recv(void *data, size_t size);

	SocketUDP(const SocketUDP& so);
public:
	friend void* cThreadFunctionB(void *param);

	SocketUDP(Logger *l);
	SocketUDP(int hS, struct sockaddr_in a, Logger *l);
	
	void init(std::string d, int p, IPType t);

	void onStart(bool wait);
	void onStop(bool eoc);

	int send(void *data, size_t size, int flag = 0);

	struct sockaddr_in getFrom() const;
	char* getPacket() const;

	void setBufsize(int size);
};