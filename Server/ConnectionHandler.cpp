#include "ConnectionHandler.h"

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
    if (msgLength >= MAX_CLIENT_MESSAGE_LEN)
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
        else
            throw std::runtime_error("Connection closed by client");
    }
}

void ConnectionHandler::setSocket(const SOCKET& socket)
{
	_socket = socket;
}