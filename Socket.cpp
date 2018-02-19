#include "Socket.h"

Socket::Socket(Logger *l) : hSocket(-1), logger(l), running(false)
{
}

Socket::Socket(int hA, struct sockaddr_in a, Logger *l) : hSocket(hA), logger(l), running(false)
{
	memcpy(&this->address, &a, sizeof(struct sockaddr_in));
}

Socket::Socket(const Socket& so)
{
}

Socket::~Socket()
{
	if (hSocket != -1)
		this->stop();
}

void Socket::start(bool wait)
{
	this->running = true;

	onStart(wait);
}

bool Socket::stop(bool eoc)
{
	if (this->hSocket == -1) // Si il est déjà fermé
		return false;

	onStop(eoc);

	if (close(this->hSocket) == -1)
	{
		this->logger->write("Erreur de fermeture de la socket: " + std::string(strerror(errno)));
		return false;
	}

	this->hSocket = -1;
	memset(&this->address, 0, sizeof(struct sockaddr_in));

	this->running = false;

	this->logger->write("Fermeture de la socket achevée !");

	return true;
}

bool Socket::stop(std::string message)
{
	if (this->hSocket == -1) // Si il est déjà fermé
		return false;
	
	this->logger->write("Fermeture du socket du a une erreur: " + message);
	
	if (this->clistener != NULL)
		this->clistener(CLOSE_ERROR_CONNECTION);

	return this->stop(false);
}

void Socket::setMessageListener(MESSAGELISTENER listener)
{
	this->mlistener = listener;
}

void Socket::setCloseListener(CLOSELISTENER listener)
{
	this->clistener = listener;
}

int Socket::getHSocket() const
{
	return this->hSocket;
}

const struct sockaddr_in* Socket::getAddress() const
{
	return &(this->address);
}

bool Socket::isConnected() const
{
	bool success =  this->hSocket != -1;
	return success;
}