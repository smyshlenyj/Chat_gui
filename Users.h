#pragma once
#include <vector>
#include "User.h"

struct Users   // read and store all user records
{
private:
	std::vector<User> users;

public:
	Users(int, std::string const&);
	Users();

	bool uniqueLogin(int, std::string const&);
	std::vector<User> listOfUsers(int, const std::string&);
	bool loginAndPasswordMatch(int, const std::string&, const std::string&); // check is login matching the password
	std::string findUserNameByLogin(int, std::string const&);
	void addUser(int, User const&);
	void refresh(int);
};