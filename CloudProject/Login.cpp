#include "Login.h"

Login::Login(const std::string& filePath)
{
	_filePath = filePath;
}
bool Login::searchUsername(const std::string& username)
{
	sqlite3* DB;
	int exit = 0;
	exit = sqlite3_open("example.db", &DB);
}
//const Login::std::string& getPassword(const std::string& username);
//bool Login::addUser(const std::string& username, const std::string& password);