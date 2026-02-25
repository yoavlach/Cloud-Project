#pragma once
#include <string>
#include <sqlite3.h>

class Login
{
public:
	Login(const std::string& filePath);
	bool searchUsername(const std::string& username);
	const std::string& getPassword(const std::string& username);
	bool addUser(const std::string& username, const std::string& password);
private:
	std::string _filePath;
};