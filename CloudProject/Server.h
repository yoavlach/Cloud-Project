#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include "UserAuth.h"

#define MSG_CODE_LEN 3
#define USERNAME_AND_PASSWORD_LEN_SIZE 2

using namespace std;

#define PORT 2424

typedef struct Packet {
	int msgCode;
	std::string username;
	std::string password;
	std::string data;
}Packet;

class Server
{
public:
	Server();
	~Server();
	void waitForClient();
	void acceptClient();
	void clientHandler();
	Packet& parseMsg(const string& msg);
private:
	const string& getMsgPart(int& iterator, string& buffer, const string& msg);
	SOCKET _socket;
	UserAuth* _userAuth;
};