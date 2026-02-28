#pragma once
#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
using namespace std;

#define MAX_CLIENT_MESSAGE_LEN 1208

class ConnectionHandler
{
public:
	void sendMessage(char* msg);
	void receiveMessage(char* buffer);
	void setSocket(const SOCKET& socket);
private:
	SOCKET _socket;
};