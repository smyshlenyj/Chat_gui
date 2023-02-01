#include "Chat.h"

Chat::Chat(std::string const& _sender, std::string const& _recipient) // universal chat constructor
{
	std::ifstream readFromDB;
	readFromDB.open("messages.mdf", std::ios::in);
	if (!readFromDB.is_open())
		std::ofstream outfile("messages.mdf"); //create file in case it's not there
	else {
		while (!readFromDB.eof())
		{
			std::string msg;

			std::getline(readFromDB, msg);
			if (!msg.empty())
			{
				std::istringstream ss(msg);
				std::string token;
				int i = 0; // iterator for while
				std::string array[3];
				while (std::getline(ss, token, '\t'))
				{
					array[i] = token;
					++i;
				}
				// showing messages linked with current users

				if (array[1] == "_all" && _recipient == "_all") // showing messages linked with current chat
					buffer.push_back("Public chat\tFrom: " + array[0] + "\tMessage: " + array[2]);

				else if (_recipient != "_all" && ((array[0] == _sender && array[1] == _recipient) || (array[0] == _recipient && array[1] == _sender)))
					buffer.push_back("From: " + array[0] + "\tTo: " + array[1] + "\tMessage: " + array[2]);
			}
		}
	}
}

void Chat::printChat() // just prints all messages in Chat object
{
	for (auto const& i : buffer)
	{
		std::cout << i << std::endl;
	}
}