#include <filesystem>
#include "Message.h"
#include "Constants.h"

Message::Message(std::string const& _sender, std::string const& _recipient, std::string const& _message) : sender(_sender), recipient(_recipient), message(_message) {}

void Message::sendMessage(int socketID,Logger& logger) // push message to data base
{
	std::string outMessage = "sendMessage\t" + this->sender + "\t" + this->recipient + "\t" + this->message;

	char outMessageChar[MESSAGE_LENGTH];
	memset(outMessageChar, 0, MESSAGE_LENGTH);
	strcpy(outMessageChar, outMessage.c_str());
	size_t bytesSent = -1;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	bytesSent = send(socketID, outMessageChar, MESSAGE_LENGTH, NULL);
#endif

#ifdef __linux__
	bytesSent = write(socketID, outMessageChar, MESSAGE_LENGTH);
#endif

	std::cout << "sendMessage:bytesSent: " << bytesSent << std::endl;
	std::cout << "sendMessage:messageSent: " << outMessageChar << std::endl;
	logger.writeLine("->" + this->recipient + ": " + this->message);
}