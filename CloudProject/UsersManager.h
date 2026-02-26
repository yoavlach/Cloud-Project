#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "sqlite3.h"

using namespace std;

#define DB_FILE_NAME "users.db";

class UsersManager
{
public:
	UsersManager();
	bool searchUsername(const std::string& username);
	void getPassword(const std::string& username, string& passwordBuffer);
	bool addUser(const std::string& username, const std::string& password);
private:
	bool checkIfDBExists();
	static int selectData(const char* s, const std::string& username);
	static int createDB(const char* s);
	static int createTable(const char* s);
	static int callback(void* NotUsed, int argc, char** argv, char** azColName);
	static int passwordCallback(void* passwordBuffer, int argc, char** argv, char** azColName);
	static int foundResultsCallback(void* data, int argc, char** argv, char** azColName);
};