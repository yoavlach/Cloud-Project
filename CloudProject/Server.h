#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include <iostream>
#include "MessageHandler.h"

#define MSG_CODE_LEN 3
#define USERNAME_AND_PASSWORD_LEN_SIZE 2
#define MAX_CLIENT_MESSAGE_LEN 1208

using namespace std;

#define PORT 2424

typedef struct Packet {
	int msgCode;
	string username;
	string password;
	string data;
}Packet;

class Server
{
public:
	Server();
	~Server();
	void waitForClient();
	void acceptClient();
	void clientHandler();
private:
	const string& getMsgPart(int& iterator, string& buffer, const string& msg);
	SOCKET _socket;
	MessageHandler _messageHandler;
};