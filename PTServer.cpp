#include "PTServer.h"

void* sThreadFunction(void *param)
{
	PTServer *pserv = (PTServer*) param;
	SocketTCP *s = NULL;
	int clientTraite = -1;

	while (pserv->running)
	{
		pserv->logger->write("Mise en attente d'un client...");

		pthread_mutex_lock(&(pserv->mutex));

		while (pserv->current == -1)
			pthread_cond_wait(&(pserv->varCond), &(pserv->mutex));

		pserv->logger->write("Prise en charge d'un client...");

		clientTraite = pserv->current;
		pserv->current = -1;
		s = pserv->sPool.at(clientTraite);
		pthread_mutex_unlock(&(pserv->mutex));

		s->start(true);

		s->stop();

		pthread_mutex_lock(&(pserv->mutex));
		pserv->sPool.at(clientTraite) = NULL;
		pthread_mutex_unlock(&(pserv->mutex));

		s = NULL;
		clientTraite = -1;

		pserv->logger->write("Le client est parti...");
	}

	return NULL;
}

PTServer::PTServer(std::string ip, int port, int mt, Logger *l, IPType t) : ServerTCP(ip, port, l, t), maxThreads(mt), current(-1)
{
	pthread_mutex_init(&(this->mutex), NULL);
	pthread_cond_init(&(this->varCond), NULL);
}

PTServer::PTServer(const PTServer& pserv) : ServerTCP(pserv)
{
}

PTServer::~PTServer()
{
	pthread_mutex_destroy(&(this->mutex));
	pthread_cond_destroy(&(this->varCond));
}

void PTServer::onStart()
{
	this->current = -1;
	this->tPool.clear();
	this->sPool.clear();

	// Lancer threads
	for (int i = 0; i < this->maxThreads; i++)
	{
		pthread_t handle;

		pthread_create(&handle, NULL, sThreadFunction, (void*)this);

		tPool.push_back(handle);
		sPool.push_back(NULL);
	}

	ServerTCP::onStart();
}

void PTServer::onClientAccept(SocketTCP* s)
{
	int j;

	pthread_mutex_lock(&(this->mutex));

	for (j = 0; j < this->maxThreads && this->sPool.at(j) != NULL; j++);

	// Plus de client
	if (j == this->maxThreads)
	{
		char buffer[100] = DENY_OF_CONNECTION;
		s->write(buffer, strlen(buffer));
		s->stop(false);
	}
	else
	{
		this->sPool.at(j) = s;
		this->current = j;
	}

	pthread_mutex_unlock(&(this->mutex));
	pthread_cond_signal(&(this->varCond));
}

void PTServer::onStop(bool eoc)
{
	pthread_mutex_lock(&(this->mutex));
	for (SOCKET_POOL::iterator it = sPool.begin(); it != sPool.end(); it++)
	{
		if (*it != NULL)
		{
			(*it)->stop(eoc);
			this->logger->write("On a clear un client !");
		}
	}
	pthread_mutex_unlock(&(this->mutex));

	ServerTCP::onStop(eoc);
}

int PTServer::getMaxThreads() const
{
	return this->maxThreads;
}

SOCKET_POOL PTServer::getSPool() const
{
	return this->sPool;
}

PTServer* PTServer::create(std::string ip, int port, int mt, Logger *l, IPType t)
{
	PTServer *serv = new PTServer(ip, port, mt, l, t);

	if (serv->socket == NULL || !serv->socket->isConnected())
	{
		delete serv;
		return NULL;
	}

	return serv;
}