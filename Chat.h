#pragma once
#include <list>
#include <fstream>
#include <sstream>
#include "Message.h"

struct Chat
{
private:
	std::list<std::string> buffer;

public:
	Chat(std::string const&, std::string const&); // universal chat constructor

	std::list<std::string> listOfMessages();
	void sendMessage(const Message&);
};