#include <iostream>
#include "Client.h"

using namespace std;

enum OPTIONS { LOGIN_OPT = 1, SIGNUP_OPT, SEND_FILE_OPT, RECEIVE_FILE_OPT, GET_LIST_OF_FILES_OPT, EXIT_OPT};
int main()
{	
	Client c;
	bool connected = false;
	int action = 0;
	try
	{
		c.connectToServer();
		connected = true;
	}
	catch (const exception& e)
	{
		cout << e.what();
	}
	if (connected)
	{
		do
		{
			cout << "Choose action:\n1) Login\n2) Sign up\nYour choice: ";
			cin >> action;
		} while (action != LOGIN_OPT && action != SIGNUP_OPT);
		switch (action)
		{
		case LOGIN_OPT:
			c.login();
			break;
		case SIGNUP_OPT:
			c.signup();
			break;
		}
		do
		{
			do
			{
				cout << "Choose action\n3) Send file\n4) Receive file\n5) Get a list of your files\n6) Exit\nYour choice: ";
				cin >> action;
			} while (action > EXIT_OPT || action < SEND_FILE_OPT);
			switch (action)
			{
			case SEND_FILE_OPT:
				c.sendFile();
				break;
			case RECEIVE_FILE_OPT:
				c.receiveFile();
				break;
			case GET_LIST_OF_FILES_OPT:
				c.getListOfFiles();
				break;
			}
		} while (action != EXIT_OPT);
		cout << "Goodbye!";
	}
	return 0;
}