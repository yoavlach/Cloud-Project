#pragma once
#include "Server.h"
#include "UsersManager.h"

enum CLIENT_PACKETS {
    LOGIN = 100,
    SIGNUP = 200,
    START_GET_FILE = 300,
    FILE_DATA_RECEIVED = 301,
    FINISHED_FILE_GETTING = 302,
    START_FILE_SENDING = 400,
    FILE_DATA = 401,
    FINISHED_FILE_SENDING = 402
};

class MessageHandler
{
public:
    MessageHandler();
    void setPacket(const Packet& p);
    void setSocket(const SOCKET& socket);
    void login();
    void signup();
    void getFile();
    void sendFile();
private:
    Packet _p;
    UsersManager _usersManager;
    SOCKET _socket;
};