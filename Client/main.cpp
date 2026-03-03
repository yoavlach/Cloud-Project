#include <iostream>
#include "Client.h"

using namespace std;

enum OPTIONS { LOGIN_OPT = 1, SIGNUP_OPT, SEND_FILE_OPT, RECEIVE_FILE_OPT, EXIT_OPT};
int main()
{	
	Client c;
	int action = 0;
	c.connectToServer();
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
			cout << "Choose action\n3) Send file\n4) Receive file\n5) Exit\nYour choice: ";
			cin >> action;
		} while (action > SEND_FILE_OPT || action > EXIT_OPT);
		switch (action)
		{
		case SEND_FILE_OPT:
			c.sendFile();
			break;
		case RECEIVE_FILE_OPT:
			c.receiveFile();
			break;
		}
	} while (action != EXIT_OPT);
	cout << "Goodbye!";
	return 0;
}