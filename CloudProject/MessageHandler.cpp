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

//void MessageHandler::getFile();
//void MessageHandler::sendFile();