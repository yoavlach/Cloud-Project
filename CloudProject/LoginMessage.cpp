#include "LoginMessage.h"

LoginMessage::LoginMessage(const std::string& username, const std::string& password, int messageCode)
	:Message(messageCode)
{
	_username = username;
	_password = password;
}

const std::string& LoginMessage::getUsername()
{
	return _username;
}

const std::string& LoginMessage::getPassword()
{
	return _password;
}