#include "ConnectionHandler.h"

void ConnectionHandler::sendMessage(const char* msg)
{
	if (send(_socket, msg, strlen(msg), 0) == SOCKET_ERROR)
		throw runtime_error("Unable to send message");
}
void ConnectionHandler::receiveMessage(char* buffer)
{
	int bytesReceived = recv(_socket, buffer, MAX_SERVER_MESSAGE_LEN - 1, 0);
	if (bytesReceived > 0)
		buffer[bytesReceived] = '\0';
	else if (bytesReceived == 0)
		throw runtime_error("Connection closed by server");
	else
		throw runtime_error("Receive failed: " + to_string(WSAGetLastError()));
}
void ConnectionHandler::setSocket(const SOCKET& socket)
{
	_socket = socket;
}

Packet& ConnectionHandler::parseMsg(const string& msg)
{
	string msgCode = "";
	Packet* p = new Packet;
	int i = 0;
	for (i = 0; i < MSG_CODE_LEN; i++)
	{
		msgCode += msg[i];
	}
	p->msgCode = stoi(msgCode);
	getMsgPart(i, p->data, msg);
	return *p;
}

const string& ConnectionHandler::getMsgPart(int& iterator, string& buffer, const string& msg)
{
	string strPartLen = "";
	int starterItVal = iterator, partLen = 0;
	for (; iterator < starterItVal + USERNAME_AND_PASSWORD_LEN_SIZE; iterator++)
		strPartLen += msg[iterator];
	partLen = stoi(strPartLen);
	starterItVal = iterator;
	for (; iterator < starterItVal + partLen; iterator++)
		buffer += msg[iterator];
	return buffer;
}