#include "MessageHandler.h"

MessageHandler::MessageHandler()
{
	_usersManager;
	_p;
}

void MessageHandler::setPacket(const Packet& p)
{
	_p = p;
}

void MessageHandler::setSocket(const SOCKET& socket)
{
	_socket = socket;
}

void MessageHandler::login()
{
	string passwordBuffer = "";
	if (!_usersManager.searchUsername(_p.username)) throw exception("Username does not exist");
	_usersManager.getPassword(_p.username, passwordBuffer);
	if (passwordBuffer != _p.password) throw exception("Incorrect password");
}
void MessageHandler::signup()
{
	if (_usersManager.searchUsername(_p.username)) throw exception("Username already exists");
	_usersManager.addUser(_p.username, _p.password);
}

void MessageHandler::getFile()
{
	ifstream f(_p.data.c_str());
	if (!f.good())
		throw exception("File doesn't exist");
}

void MessageHandler::sendFile()
{
	ifstream f(_p.data.c_str());
	if (f.good())
		throw exception("File already exists");
}

Packet& MessageHandler::parseMsg(const string& msg)
{
	string msgCode = "";
	Packet* p = new Packet;
	int i = 0;
	for (i = 0; i < MSG_CODE_LEN; i++)
	{
		msgCode += msg[i];
	}
	p->msgCode = stoi(msgCode);
	getMsgPart(i, p->username);
	getMsgPart(i, p->password);
	getMsgPart(i, p->data);
	return *p;
}

const string& MessageHandler::getMsgPart(int& iterator, string& buffer, const string& msg)
{
	string strPartLen = "";
	int starterItVal = iterator, partLen = 0;
	for (; iterator < starterItVal + USERNAME_AND_PASSWORD_LEN_SIZE; iterator++)
	{
		strPartLen += msg[iterator];
	}
	partLen = stoi(strPartLen);
	starterItVal = iterator;
	for (; iterator < starterItVal + partLen; iterator++)
	{
		buffer += msg[iterator];
	}
	return buffer;
}