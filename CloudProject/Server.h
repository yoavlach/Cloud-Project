#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include <iostream>
#include "MessageHandler.h"

#define MSG_CODE_LEN 3
#define USERNAME_AND_PASSWORD_LEN_SIZE 2
#define MAX_CLIENT_MESSAGE_LEN 1208

enum CLIENT_PACKETS {
	LOGIN = 100,
	SIGNUP = 200
};

enum SERVER_PACKETS {
	LOGIN_SUCCESSFUL = 500,
	USER_DOESNT_EXIST = 501,
	INCORRECT_PASSWORD = 502,
	SIGN_UP_SUCCESSFUL = 600,
	USERNAME_ALREADY_EXISTS = 601,
	FILE_DOES_NOT_EXIST = 701,
	FILE_WITH_THIS_NAME_ALREADY_EXISTS = 801,
	GENERAL_INVALID_MESSAGE = 900
};

enum SERVER_AND_CLIENT_PACKETS
{
	READY_TO_RECEIVE_FILE = 300,
	FILE_DATA = 301,
	FINISHED_RECEIVING_FILE = 302,
	READY_TO_SEND_FILE = 400,
	FINISHED_SENDING_FILE = 401,
	FILE_DATA_RECEIVED = 402,
	ERROR_WHILE_RECEIVING_FILE = 403
};

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
	ConnectionHandler _connectionHandler;
};