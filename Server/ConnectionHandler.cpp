#include "ConnectionHandler.h"

void ConnectionHandler::sendMessage(const char* msg)
{
	if (send(_socket, msg, strlen(msg), 0) == SOCKET_ERROR)
		throw runtime_error("Unable to send message");
}
void ConnectionHandler::receiveMessage(char* buffer)
{
	int bytesReceived = recv(_socket, buffer, MAX_CLIENT_MESSAGE_LEN - 1, 0);
	if (bytesReceived > 0)
		buffer[bytesReceived] = '\0';
	else if (bytesReceived == 0)
		throw runtime_error("Connection closed by client");
	else
		throw runtime_error("Receive failed: " + to_string(WSAGetLastError()));
}
void ConnectionHandler::setSocket(const SOCKET& socket)
{
	_socket = socket;
}