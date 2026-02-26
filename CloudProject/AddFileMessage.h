#pragma once
#include "Message.h"
#include <string>

class LoginMessage : protected Message
{
public:
	LoginMessage(const std::string& username, const std::string& password, int messageCode);
	const std::string& getUsername();
	const std::string& getPassword();
private:
	std::string _username;
	std::string _password;
};