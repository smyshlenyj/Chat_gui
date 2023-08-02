#pragma once
#include <vector>
#include <string>
#include "User.h"

struct Users   // read and store all user records
{
private:
	std::vector<User> users;

public:
	Users(int, std::string const&);
	Users();

	bool uniqueLogin(int, std::string const&);
	void printUsers(); // just prints all user names and logins
	std::vector<User> listOfUsers();
	//bool loginAndPasswordMatch(User const&); // check is login matching the password
	bool loginAndPasswordMatch(int, const std::string&, const std::string&); // check is login matching the password
	std::string findUserNameByLogin(int, std::string const&);
	void addUser(int, User const&);
	void refresh(int);
};