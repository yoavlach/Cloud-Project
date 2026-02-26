#pragma once
#include <string>
#include <iostream>
#include "UsersManager.h"

using namespace std;

class UserAuth
{
public:
	UserAuth(const string& username, const string& password);
	void login();
	void signup();
	void setUsername(const string& username);
	void setPassword(const string& password);
private:
	string _username;
	string _password;
	UsersManager _userManager;
};