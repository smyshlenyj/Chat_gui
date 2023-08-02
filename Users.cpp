#include "Users.h"
#include "User.h"
#include "Connection.h"
#include <iostream>
#include <string.h>
#include <sstream>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#pragma comment(lib, "ws2_32.lib") // обеспечивает доступ к некоторым функциям
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif


Users::Users(int socketID, std::string const& login)
{
	refresh(socketID);
}

Users::Users() {}

bool Users::uniqueLogin(int socketID, std::string const& login) // check login for uniqueness
{
	std::string message = "uniqueLogin\t" + login;
	char msg[MESSAGE_LENGTH];
	strcpy(msg, message.c_str());

	size_t bytesSent = -1;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	bytesSent = send(socketID, msg, MESSAGE_LENGTH, NULL);
#endif

#ifdef __linux__
	bytesSent = write(socketID, msg, messageSize);
#endif

	if (bytesSent == -1)
		std::cout << "Error sending query!" << std::endl;

	char reply[MESSAGE_LENGTH];
	memset(reply, 0, MESSAGE_LENGTH);

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	recv(socketID, reply, MESSAGE_LENGTH, NULL);
#endif

#ifdef __linux__
	read(socketID, reply, sizeof(reply));
#endif

	std::cout << "Reply from server: " << reply << std::endl;
	bool result = (strncmp("true", reply, 4) == 0);
	return result;
}

void Users::printUsers() // just prints all user names and logins
{
	for (auto i : users)
		std::cout << "User: " << i.getLogin() << ",\t\t Name: " << i.getUserName() << '\n';
}

std::vector<User> Users::listOfUsers(int socketID, const std::string& login) // just prints all user names and logins
{
	return users;
}

bool Users::loginAndPasswordMatch(int socketID, const std::string& login, const std::string& password)
{
	std::string hashedPassword = hashPassword(password);
	std::string message = std::string("signIn") + '\t' + login + '\t' + hashedPassword + '\0';
	std::cout << message << std::endl;
	char msg[MESSAGE_LENGTH];
	memset(msg, 0, MESSAGE_LENGTH);
	strcpy(msg, message.c_str());
	std::cout << msg << std::endl;
	size_t bytesSent = -1;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	bytesSent = send(socketID, msg, MESSAGE_LENGTH, NULL);
#endif

#ifdef __linux__
	bytesSent = write(socketID, buffer, messageSize);
#endif

	if (bytesSent == -1)
	{
		std::cout << "Error sending query!" << std::endl;
}
	char reply[MESSAGE_LENGTH];
	memset(reply, 0, MESSAGE_LENGTH);

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	recv(socketID, reply, MESSAGE_LENGTH, NULL);
#endif

#ifdef __linux__
	read(socketID, reply, sizeof(reply));
#endif

	std::cout << "Reply from server: " << reply << std::endl;
	bool result = (strncmp("true", reply, 4) == 0);
	return result;
}

std::string Users::findUserNameByLogin(int socketID, const std::string& login)
{
	std::string message = "getUserName\t" + login;
	size_t messageSize = message.size() + 1;
	char msg[MESSAGE_LENGTH];
	strcpy(msg, message.c_str());
	size_t bytesSent = -1;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	bytesSent = send(socketID, msg, MESSAGE_LENGTH, NULL);
#endif

#ifdef __linux__
	bytesSent = write(socketID, msg, MESSAGE_LENGTH);
#endif

	if (bytesSent == -1)
		std::cout << "Error sending query!" << std::endl;

	char userName[MESSAGE_LENGTH];
	memset(userName, 0, MESSAGE_LENGTH);

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	recv(socketID, userName, MESSAGE_LENGTH, NULL);
#endif

#ifdef __linux__
	read(socketID, userName, MESSAGE_LENGTH);
#endif

	std::cout << "Reply from server findUserNameByLogin: " << userName << std::endl;
	return userName;
}

void Users::addUser(int socketID, User const& user)
{
	std::string usr = "addUser\t" + user.getLogin() + "\t" + user.getPassword() + "\t" + user.getUserName();
	char msg[MESSAGE_LENGTH];
	memset(msg, 0, MESSAGE_LENGTH);
	strcpy(msg, usr.c_str());

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	send(socketID, msg, MESSAGE_LENGTH, NULL);
#endif

#ifdef __linux__
	write(socketID, msg, MESSAGE_LENGTH);
#endif
}

void Users::refresh(int socketID)
{
	std::string message = "getUsers\t";
	size_t messageSize = message.size() + 1;
	char msg[MESSAGE_LENGTH];
	memset(msg, 0, MESSAGE_LENGTH);
	strcpy(msg, message.c_str());
	size_t bytes = -1;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	bytes = send(socketID, msg, MESSAGE_LENGTH, NULL);
#endif

#ifdef __linux__
	bytes = write(socketID, msg, MESSAGE_LENGTH);
#endif

	if (bytes == -1)
		std::cout << "Error sending a query!" << std::endl;

	char reply[MESSAGE_LENGTH];
	memset(reply, 0, MESSAGE_LENGTH);

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	recv(socketID, reply, MESSAGE_LENGTH, NULL);
#endif

#ifdef __linux__
	read(socketID, reply, MESSAGE_LENGTH);
#endif

	int usersQuantity = std::atoi(reply);
	std::cout << "Reply from server getUsers: " << usersQuantity << std::endl;
	std::vector<User> listOfUsers;
	for (int i = 0; i < usersQuantity; ++i)
	{
		char user[MESSAGE_LENGTH];
		memset(user, 0, MESSAGE_LENGTH);

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
		recv(socketID, user, MESSAGE_LENGTH, NULL);
#endif

#ifdef __linux__
		read(socketID, user, MESSAGE_LENGTH);
#endif

		std::vector<std::string> array;
		std::stringstream ss(user);
		std::string tmp;
		while (std::getline(ss, tmp, '\t'))
		{
			array.push_back(tmp);
		}

		listOfUsers.push_back(User(array[0], "", array[1]));
	}
	users = listOfUsers;
}