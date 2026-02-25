#include <iostream>
#include "Login.h"

int main()
{
	Login l("");
	char s;
	l.insertData(&s, "Yoav", "123");
	l.selectData(&s, "Yoav");
}