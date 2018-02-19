 #include "SocketTCP.h"

void* cThreadFunctionA(void *param)
{
	SocketTCP* socket = (SocketTCP*) param;
	char temp;

	while (socket->running)
	{
		bool success = socket->read(&temp, (size_t)1, MSG_PEEK);

		if (socket->running && success && socket->mlistener != NULL)
			socket->mlistener(socket);
	}

	return NULL;
}

int SocketTCP::ssend(void *data, size_t size, int flag)
{
	int writed;

	if ((writed = send(this->hSocket, data, size, flag)) == -1)
	{
		this->stop("Erreur sur le send de la socket: " + std::string(strerror(errno)));
		return false;
	}

	return writed;
}

int SocketTCP::rrecv(void *data, size_t size, int flag)
{
	char buffer[100];
	int s = recv(this->hSocket, buffer, sizeof(char) * 100, MSG_PEEK);
	buffer[s] = 0;

	if (strcmp(buffer, END_OF_CONNECTION) == 0 || strcmp(buffer, DENY_OF_CONNECTION) == 0)
	{
		// Comme il y avait bien un message pour la socket sur le buffer, on le relis sans le MSG_PEEK pour l'enlever du buffer
		recv(this->hSocket, buffer, strlen(buffer), 0);

		if (this->clistener != NULL)
			this->clistener(strcmp(buffer, END_OF_CONNECTION) == 0 ? CLOSE_END_CONNECTION : CLOSE_DENY_CONNECTION);
		
		this->stop(false);
		return false;
	}

	size_t curSize = 0;
	int byteRead = 0;

	do
	{
		if ((byteRead = recv(this->hSocket, ((char*)data) + curSize, size - curSize, flag)) == -1)
		{
			this->stop("Erreur sur le recv de la socket: " + std::string(strerror(errno)));
			return false;
		}
		else
			curSize += byteRead;


	} while (byteRead != 0 && byteRead != -1 && curSize < size);

	return byteRead;
}

void SocketTCP::init(std::string data, int port, IPType type)
{
	this->hSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (hSocket == -1)
	{
		this->logger->write("Erreur de création de la socket: " + std::string(strerror(errno)));
		return;
	}

	this->logger->write("Création socket OK.");

	struct hostent *infosHost;
	struct sockaddr_in adresseSocket;

	if ((infosHost = gethostbyname(data.c_str())) == 0)
	{
		this->stop("Erreur d'acquisition d'infos sur le host: " + std::string(strerror(errno)));
		return;
	}
	
	this->logger->write("Acquisition infos host OK");
	
	/* Préparation de la structure sockaddr_in */
	memset(&adresseSocket, 0, sizeof(struct sockaddr_in));
	adresseSocket.sin_family = AF_INET;
	adresseSocket.sin_port = htons(port); 	/* conversion numéro de port au format réseau */
	
	memcpy(&adresseSocket.sin_addr, infosHost->h_addr,infosHost->h_length);

	memcpy(&this->address, &adresseSocket, sizeof(struct sockaddr_in));
}

void SocketTCP::link()
{
	/* Le système prend connaissance de l'adresse et du port de la socket */
	if (bind(this->hSocket, (struct sockaddr *)&this->address, sizeof(struct sockaddr_in)) == -1)
	{
		this->stop("Erreur sur le bind de la socket: " + std::string(strerror(errno)));
		return;
	}

	this->logger->write("Bind adresse et port socket OK");
}

void SocketTCP::onStart(bool wait)
{
	pthread_create(&(this->thread), NULL, cThreadFunctionA, (void*)this);

	if (wait)
		pthread_join(this->thread, NULL);
}

void SocketTCP::onStop(bool eoc)
{
	char buffer[100] = END_OF_CONNECTION;

	if (eoc)
	{
		this->write(buffer, strlen(buffer));
		this->logger->write("Fermeture et envoi de fin de connection!");
	}
}

int SocketTCP::write(void *data, size_t size, bool writeSize, int flag)
{
	if (writeSize)
	{
		uint32_t ut = htonl(size);
		if (this->ssend((void*)&ut, 4, flag) <= 0)
			return false;
	}

	return this->ssend(data, size, flag);
}

int SocketTCP::read(void *data, size_t size, int flag)
{
	return this->rrecv(data, size, flag);
}

int SocketTCP::read(void *data, char separator, int flag)
{
	bool found = false;
	size_t curSize = 0;

	do
	{
		if (this->rrecv(((char*)data) + curSize * sizeof(char), sizeof(char), flag) > 0)
		{
			if (*(((char*)data) + curSize * sizeof(char)) == separator)
				found = true;

			curSize++;
		}


	} while (!found);

	return true;
}

int SocketTCP::read(void *data, int flag)
{
	char buffer[4];

	if (!this->rrecv((void*)buffer, 4, flag))
		return false;

	uint32_t ut = *((uint32_t*)buffer);

	int size = ntohl(ut);

	return this->read(data, (size_t)size, flag);
}

SocketTCP::SocketTCP(Logger *l) : Socket(l)
{
	
}

SocketTCP::SocketTCP(int hA, struct sockaddr_in a, Logger *l) : Socket(hA, a, l)
{
}

SocketTCP::SocketTCP(const SocketTCP& so) : Socket(so)
{
}