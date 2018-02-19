#pragma once

#include "ServerTCP.h"
#include <vector>

typedef std::vector<pthread_t> THREAD_POOL;
typedef std::vector<SocketTCP*> SOCKET_POOL;

void* sThreadFunction(void *param); 

class PTServer : public ServerTCP
{
private:
	int maxThreads;
	THREAD_POOL tPool;
	SOCKET_POOL sPool;
	int current;
	pthread_mutex_t mutex;
	pthread_cond_t varCond;

	PTServer(std::string ip, int port, int mt, Logger *l, IPType t = IPType::UNICAST);
	PTServer(const PTServer& pserv);
public:
	friend void* sThreadFunction(void* param);

	~PTServer();

	void onStart();
	void onClientAccept(SocketTCP* s);
	void onStop(bool eoc);

	int getMaxThreads() const;
	SOCKET_POOL getSPool() const;

	static PTServer* create(std::string ip, int port, int mt, Logger *l, IPType t = IPType::UNICAST);
};