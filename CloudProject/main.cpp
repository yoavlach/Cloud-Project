#include <iostream>
#include "Login.h"

int main()
{
	Login l("");
	char s;
	//l.insertData(&s, "Yoav", "123");
	//l.searchUsername("Yoav");
	string password = "";
	l.getPassword("Yoav", password);
	cout << password << endl;
}