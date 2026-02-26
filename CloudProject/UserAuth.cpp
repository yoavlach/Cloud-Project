#include "UserAuth.h"

UserAuth::UserAuth(const string& username, const string& password)
{
	_username = username;
	_password = password;
	_userManager;
}

void UserAuth::login()
{
	string passwordBuffer = "";
	if (!_userManager.searchUsername(_username)) throw exception("Username does not exist");
	_userManager.getPassword(_username, passwordBuffer);
	if (passwordBuffer != _password) throw exception("Incorrect password"); 
}

void UserAuth::signup()
{
	if (_userManager.searchUsername(_username)) throw exception("Username already exists");
	_userManager.addUser(_username, _password);
}

void UserAuth::setUsername(const string& username)
{
	_username = username;
}

void UserAuth::setPassword(const string& password)
{
	_password = password;
}