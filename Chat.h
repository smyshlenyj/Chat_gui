#pragma once
#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include "Message.h"
#include "Constants.h"

struct Chat
{
private:
	std::list<std::string> buffer;

public:
	Chat(int, std::string const&, std::string const&); // universal chat constructor
	Chat(std::list<std::string> const&);
	Chat() = default;

	std::list<std::string> getChat(int, std::string const&, std::string const&);
	void printChat(); // just prints all messages in Chat object
	std::list<std::string> listOfMessages();
	//void sendMessage(int, const Message&);
};