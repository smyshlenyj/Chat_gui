#include <filesystem>
#include "Message.h"

Message::Message(std::string const& _sender, std::string const& _recipient, std::string const& _message) : sender(_sender), recipient(_recipient), message(_message) {}

void Message::sendMessage() // push message to data base
{
#ifdef __linux__
	std::string home = getenv("HOME");
	std::filesystem::path messagesPath{ home + "/.stackmessenger/messages.mdf" };
	std::ofstream out(messagesPath, std::ios::app);
	if (out.is_open())
		out << sender << "\t" << recipient << "\t" << message << std::endl;

	out.close();
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	std::ofstream out("messages.mdf", std::ios::app);
	if (out.is_open())
		out << sender << "\t" << recipient << "\t" << message << std::endl;
	out.close();
#endif
}