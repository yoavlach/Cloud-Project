#include "UsersManager.h"

UsersManager::UsersManager()
{
	string name = DB_FILE_NAME;
	if (!checkIfDBExists())
	{
		ofstream outfile(name.c_str());
        createDB(name.c_str());
        createTable(name.c_str());
	}
}
bool UsersManager::searchUsername(const string& username)
{
	sqlite3* DB;
	char* messageError = nullptr;
	int exit = sqlite3_open("users.db", &DB);
	string sql = "SELECT EXISTS(SELECT 1 FROM USERS WHERE USERNAME='" + username + "');";
	int found = 0;
	exit = sqlite3_exec(DB, sql.c_str(), foundResultsCallback, &found, &messageError);
	if (exit != SQLITE_OK)
	{
		std::cout << "SQL Error: " << messageError << "\n";
		sqlite3_free(messageError);
	}
	return found;
}


void UsersManager::getPassword(const string& username, string& passwordBuffer)
{
	sqlite3* DB;
	char* messageError = nullptr;
	int exit = sqlite3_open("users.db", &DB);
	string sql = "SELECT PASSWORD FROM users WHERE USERNAME = '" + username + "';";
	if (searchUsername(username))
	{
		exit = sqlite3_exec(DB, sql.c_str(), passwordCallback, &passwordBuffer, &messageError);
		if (exit != SQLITE_OK)
		{
			cerr << "Error Select: " << messageError << endl;
			sqlite3_free(messageError);
		}
	}
	sqlite3_close(DB);
}

int UsersManager::passwordCallback(void* passwordBuffer, int argc, char** argv, char** azColName)
{
	string* res = (string*)passwordBuffer;
	if (argc == 1) *res = argv[0];
	return 0;
}

bool UsersManager::addUser(const string& username, const string& password)
{
	sqlite3* DB;
	char* messaggeError;
	int exit = sqlite3_open("users.db", &DB);
	string sql("INSERT INTO USERS (USERNAME, PASSWORD) VALUES('" + username + "', '" + password + "');");
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
	if (exit != SQLITE_OK)
	{
		cerr << "Error Insert" << endl;
		sqlite3_free(messaggeError);
	}
	return 0;
}

bool UsersManager::checkIfDBExists()
{
	string name = DB_FILE_NAME;
	ifstream f(name.c_str());
	return f.good();
}

int UsersManager::createDB(const char* s)
{
	sqlite3* DB;
	int exit = sqlite3_open(s, &DB);
	sqlite3_close(DB);
	return 0;
}

int UsersManager::createTable(const char* s)
{
	sqlite3* DB = nullptr;
	string sql = "CREATE TABLE IF NOT EXISTS USERS("
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
			cerr << "Error Create Table" << endl;
			sqlite3_free(messageError);
		}
		sqlite3_close(DB);
	}
	catch (const exception & e)
	{
		cerr << e.what();
	}
	return 0;
}

int UsersManager::selectData(const char* s, const string& username)
{
	sqlite3* DB;
	char* messageError = nullptr;
	int exit = sqlite3_open("users.db", &DB);
	string sql = "SELECT * FROM users WHERE USERNAME = '" + username + "';";
	exit = sqlite3_exec(DB, sql.c_str(), callback, NULL, &messageError);
	if (exit != SQLITE_OK) 
	{
		cerr << "Error Select: " << messageError << endl;
		sqlite3_free(messageError);
	}
	sqlite3_close(DB);
	return 0;
}

int UsersManager::callback(void* NotUsed, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++) 
	{
		cout << azColName[i] << ": " << argv[i] << endl;
	}
	cout << endl;
	return 0;
}

int UsersManager::foundResultsCallback(void* data, int argc, char** argv, char** azColName)
{
	int* found = (int*)data;
	if (argc > 0 && argv[0] != nullptr) *found = std::stoi(argv[0]);
	return 0;
}