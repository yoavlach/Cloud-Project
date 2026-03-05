#include "ConnectionHandler.h"

#include <winsock2.h>
#include <stdexcept>
#include <string>

void ConnectionHandler::sendMessage(const char* msg)
{
    uint32_t msgLength = strlen(msg);
    uint32_t msgLengthNetwork = htonl(msgLength);
    sendAll(reinterpret_cast<const char*>(&msgLengthNetwork), sizeof(msgLengthNetwork));
    sendAll(msg, msgLength);
}

void ConnectionHandler::receiveMessage(char* buffer)
{
    uint32_t msgLengthNetwork;
    receiveAll(reinterpret_cast<char*>(&msgLengthNetwork), sizeof(msgLengthNetwork));
    uint32_t msgLength = ntohl(msgLengthNetwork);
    if (msgLength >= MAX_SERVER_MESSAGE_LEN)
        throw std::runtime_error("Incoming message (" + std::to_string(msgLength) + " bytes) exceeds maximum allowed length.");
    receiveAll(buffer, msgLength);
    buffer[msgLength] = '\0';
}
void ConnectionHandler::sendAll(const char* data, int length)
{
    int totalSent = 0;
    while (totalSent < length) {
        int bytesSent = send(_socket, data + totalSent, length - totalSent, 0);

        if (bytesSent == SOCKET_ERROR)
            throw std::runtime_error("Send failed: " + std::to_string(WSAGetLastError()));
        totalSent += bytesSent;
    }
}

void ConnectionHandler::receiveAll(char* buffer, int length)
{
    int totalReceived = 0;
    while (totalReceived < length) {
        int bytesReceived = recv(_socket, buffer + totalReceived, length - totalReceived, 0);

        if (bytesReceived > 0)
            totalReceived += bytesReceived;
        else if (bytesReceived == 0)
            throw std::runtime_error("Connection gracefully closed by peer");
        else
            throw std::runtime_error("Receive failed: " + std::to_string(WSAGetLastError()));
    }
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
	for (; iterator < starterItVal + SERVER_DATA_LEN_SIZE; iterator++)
		strPartLen += msg[iterator];
	partLen = stoi(strPartLen);
	starterItVal = iterator;
	for (; iterator < starterItVal + partLen; iterator++)
		buffer += msg[iterator];
	return buffer;
}