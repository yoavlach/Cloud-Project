#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "sqlite3.h"

#define DB_FILE_NAME "users.db";

class Login
{
public:
	Login(const std::string& filePath);
	bool searchUsername(const std::string& username);
	const std::string& getPassword(const std::string& username);
	bool addUser(const std::string& username, const std::string& password);
	bool checkIfDBExists();
	static int selectData(const char* s, const std::string& username);
	static int insertData(const char* s, const std::string& username, const std::string& password);
private:
	static int createDB(const char* s);
	static int createTable(const char* s);
	static int callback(void* NotUsed, int argc, char** argv, char** azColName);
	std::string _filePath;
	sqlite3* DB;
};