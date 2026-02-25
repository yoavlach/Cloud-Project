#include "Login.h"

Login::Login(const std::string& filePath)
{
	_filePath = filePath;
	std::string name = DB_FILE_NAME;
	sqlite3* DB;
	if (!checkIfDBExists())
	{
		std::ofstream outfile(name.c_str());
        createDB(name.c_str());
        createTable(name.c_str());
	}
}
//bool Login::searchUsername(const std::string& username)
//{
//	sqlite3* DB;
//	int exit = 0;
//	exit = sqlite3_open("example.db", &DB);
//}
//const Login::std::string& getPassword(const std::string& username);
//bool Login::addUser(const std::string& username, const std::string& password);

bool Login::checkIfDBExists()
{
	std::string name = DB_FILE_NAME;
	std::ifstream f(name.c_str());
	return f.good();
}
int Login::createDB(const char* s)
{
	sqlite3* DB;
	int exit = sqlite3_open(s, &DB);
	sqlite3_close(DB);
	return 0;
}

int Login::createTable(const char* s)
{
	sqlite3* DB = nullptr;
	std::string sql = "CREATE TABLE IF NOT EXISTS USERS("
		"USERNAME TEXT NOT NULL PRIMARY KEY,"
		"PASSWORD TEXT NOT NULL"
		");";
	try
	{
		int exit = 0;
		char* messageError = nullptr;
		exit = sqlite3_open("users.db", &DB);
		exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
		if (exit != SQLITE_OK)
		{
			std::cerr << "Error Create Table" << std::endl;
			sqlite3_free(messageError);
		}
		sqlite3_close(DB);
	}
	catch (const std::exception & e)
	{
		std::cerr << e.what();
	}
	return 0;
}

int Login::insertData(const char* s, const std::string& username, const std::string& password)
{
	sqlite3* DB;
	char* messaggeError;
	int exit = sqlite3_open("users.db", &DB);
	std::string sql("INSERT INTO USERS (USERNAME, PASSWORD) VALUES('" + username + "', '" + password + "');");
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
	if (exit != SQLITE_OK) 
	{
		std::cerr << "Error Insert" << std::endl;
		sqlite3_free(messaggeError);
	}
	return 0;
}
int Login::selectData(const char* s, const std::string& username)
{
	sqlite3* DB;
	int exit = sqlite3_open("users.db", &DB);
	std::string sql = "SELECT * FROM USERS WHERE USERNAME = '" + username + "'";
	char* messageError = nullptr;
	exit = sqlite3_exec(DB, sql.c_str(), callback, NULL, &messageError);
	if (exit != SQLITE_OK) 
	{
		std::cerr << "Error Select: " << messageError << std::endl;
		sqlite3_free(messageError);
	}
	sqlite3_close(DB);
	return 0;
}

int Login::callback(void* NotUsed, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++) 
	{
		std::cout << azColName[i] << ": " << argv[i] << std::endl;
	}
	std::cout << std::endl;
	return 0;
}