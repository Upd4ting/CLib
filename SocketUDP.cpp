#include "SocketUDP.h"

void* cThreadFunctionB(void *param)
{
	SocketUDP* socket = (SocketUDP*) param;

	socket->packet = new char[socket->bufsize];

	while (socket->running)
	{
		bool success = socket->recv(socket->packet, socket->bufsize);

		if (socket->running && success && socket->mlistener != NULL)
		{
			socket->mlistener(socket);
		}
	}

	return NULL;
}

int SocketUDP::send(void *data, size_t size, int flag)
{
	int writed;

	socklen_t fromsize = sizeof(this->address);

	if ((writed = sendto(this->hSocket, data, size, flag, (struct sockaddr*)&this->address, fromsize)) == -1)
	{
		this->stop("Erreur sur le send de la socket: " + std::string(strerror(errno)));
		return false;
	}

	this->logger->write("Send socket OK");

	return writed;
}

int SocketUDP::recv(void *data, size_t size)
{
	struct sockaddr_in s_addr;
	socklen_t fromsize = sizeof(s_addr);

	int byteRead = 0;

	if ((byteRead = recvfrom(this->hSocket, data, size, 0, (struct sockaddr*)&s_addr, &fromsize)) == -1)
	{
		this->stop("Erreur sur le recv de la socket: " + std::string(strerror(errno)));
		return false;
	}

	((char*)data)[byteRead] = 0;

	return byteRead;
}

void SocketUDP::init(std::string d, int p, IPType t)
{
	this->hSocket = socket(AF_INET, t == IPType::MULTICAST ? SOCK_DGRAM : SOCK_STREAM, 0);

	if (hSocket == -1)
	{
		this->logger->write("Erreur de création de la socket: " + std::string(strerror(errno)));
		return;
	}

	this->logger->write("Création socket OK.");

	struct sockaddr_in adresseSocket;

	struct hostent *infosHost;

	if ((infosHost = gethostbyname(d.c_str())) == 0)
	{
		this->stop("Erreur d'acquisition d'infos sur le host: " + std::string(strerror(errno)));
		return;
	}

	this->logger->write("Acquisition infos host OK");

	memset(&adresseSocket, 0, sizeof(struct sockaddr_in));

	if (t == IPType::UNICAST)
	{		
		/* Préparation de la structure sockaddr_in */
		adresseSocket.sin_family = AF_INET;
		adresseSocket.sin_port = htons(p); 	/* conversion numéro de port au format réseau */
		
		memcpy(&adresseSocket.sin_addr, infosHost->h_addr, infosHost->h_length);

		/* Le système prend connaissance de l'adresse et du port de la socket */
		if (bind(this->hSocket, (struct sockaddr *)&adresseSocket, sizeof(struct sockaddr_in)) == -1)
		{
			this->stop("Erreur sur le bind de la socket: " + std::string(strerror(errno)));
			return;
		}
	}
	else // Préparation de la socket pour du multicast
	{
		int optReUseAddr = 1;
		struct ip_mreq mreq;
	    
		setsockopt(this->hSocket, SOL_SOCKET, SO_REUSEADDR, &optReUseAddr, sizeof(int));

		adresseSocket.sin_addr.s_addr = htonl(INADDR_ANY);
		adresseSocket.sin_port = htons(p);
		adresseSocket.sin_family = AF_INET;

		if (bind(this->hSocket, (struct sockaddr *) (&adresseSocket), sizeof(adresseSocket)) < 0)
		{
			perror("CREATE CONN : erreur lors du bind "); 
			return;
		}

		memcpy(&mreq.imr_multiaddr, infosHost->h_addr, infosHost->h_length);

		mreq.imr_interface.s_addr = htonl(INADDR_ANY);

		if (setsockopt(this->hSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
		{
			perror("setsockopt IP_ADD_MEMBERSHIP"); 
			return;
		}

		int ttl = 3;

		if (setsockopt(this->hSocket, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(int)) < 0)
		{
			perror("setsockopt IP_MULTICAST_TTL"); 
			return;
		}
	}

	memcpy(&this->address, &adresseSocket, sizeof(struct sockaddr_in));
}

void SocketUDP::onStart(bool wait)
{
	pthread_create(&(this->thread), NULL, cThreadFunctionB, (void*)this);

	if (wait)
		pthread_join(this->thread, NULL);
}

void SocketUDP::onStop(bool eoc)
{
}

struct sockaddr_in SocketUDP::getFrom() const
{
	return from;
}

char* SocketUDP::getPacket() const
{
	return packet;
}

SocketUDP::SocketUDP(Logger *l) : Socket(l), bufsize(4096)
{
}

SocketUDP::SocketUDP(int hA, struct sockaddr_in a, Logger *l) : Socket(hA, a, l), bufsize(4096)
{
}

SocketUDP::SocketUDP(const SocketUDP& so) : Socket(so)
{
}