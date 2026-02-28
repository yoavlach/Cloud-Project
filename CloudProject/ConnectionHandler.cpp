#include "ConnectionHandler.h"

void ConnectionHandler::sendMessage(char* msg)
{
	if (send(_socket, msg, sizeof(msg), 0) == SOCKET_ERROR)
		throw exception("Unable to send message");
}
void ConnectionHandler::receiveMessage(char* buffer)
{
	int bytesReceived = recv(_socket, buffer, sizeof(buffer) - 1, 0);
	if (bytesReceived > 0)
		buffer[bytesReceived] = '\0';
	else if (bytesReceived == 0)
		throw exception("Connection closed by client");
	else
		throw exception("Receive failed: " + WSAGetLastError());
}
void ConnectionHandler::setSocket(const SOCKET& socket)
{
	_socket = socket;
}