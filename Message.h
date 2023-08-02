#pragma once
#include <fstream>
#include "Connection.h"

struct Message
{
private:
	std::string sender, recipient, message;

public:
	Message(std::string const&, std::string const&, std::string const&);
	void sendMessage(int);

	//	std::string getSender()    const noexcept;
	//	std::string getRecipient() const noexcept;
	//	std::string getMessage()   const noexcept;
};