#include "Chat.h"
#include "Connection.h"
#include "Constants.h"

Chat::Chat(int socketID, std::string const &_sender, std::string const &_recipient) // universal chat constructor
{
	buffer = getChat(socketID, _sender, _recipient);
}

std::list<std::string> Chat::getChat(int socketID, std::string const &_sender, std::string const &_recipient)
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	std::string message = "getChat\t" + _sender + "\t" + _recipient;
	char msg[MESSAGE_LENGTH];
	memset(msg, 0, MESSAGE_LENGTH);
	strcpy(msg, message.c_str());
	size_t bytesSent = send(socketID, msg, MESSAGE_LENGTH, NULL);
	if (bytesSent == -1)
		std::cout << "Error sending data" << std::endl;

	char reply[MESSAGE_LENGTH];
	memset(reply, 0, MESSAGE_LENGTH);
	size_t bytesRead = recv(socketID, reply, MESSAGE_LENGTH, NULL);
	std::cout << "getChat:messagesQuantity:bytesRead: " << bytesRead << std::endl;
	int messagesQuantity = std::atoi(reply);
	std::cout << "Reply from server getChat:messagesQuantity: " << messagesQuantity << std::endl;
	std::list<std::string> messages;

	for (int i = 0; i < messagesQuantity; ++i)
	{
		char messageFromServer[MESSAGE_LENGTH];
		memset(messageFromServer, 0, MESSAGE_LENGTH);
		bytesRead = 0;
		int bytesSent = 0;
		bytesRead = recv(socketID, messageFromServer, MESSAGE_LENGTH, NULL);

		std::cout << "getChat:MESSAGE:bytesRead: " << bytesRead << std::endl;
		std::cout << "getChat:RECEIVED MSG:messageFromAServer: " << messageFromServer << std::endl;
		/* std::vector<std::string> array;
		std::stringstream ss(message);
		std::string tmp; */

		messages.push_back(messageFromServer);
	}
	return messages;
#endif
#ifdef __linux__
	std::string message = "getChat\t" + _sender + "\t" + _recipient;
	// size_t messageSize = message.size();
	char msg[MESSAGE_LENGTH];
	memset(msg, 0, MESSAGE_LENGTH);
	strcpy(msg, message.c_str());
	size_t bytes = write(socketID, msg, MESSAGE_LENGTH);

	if (bytes == -1)
		std::cout << "Error sending data" << std::endl;

	char reply[MESSAGE_LENGTH];
	memset(reply, 0, MESSAGE_LENGTH);
	size_t bytesRead = read(socketID, reply, MESSAGE_LENGTH);
	std::cout << "getChat:messagesQuantity:bytesRead: " << bytesRead << std::endl;
	int messagesQuantity = std::atoi(reply);
	std::cout << "Reply from server getChat:messagesQuantity: " << messagesQuantity << std::endl;
	std::list<std::string> messages;

	for (int i = 0; i < messagesQuantity; ++i)
	{
		char messageFromServer[MESSAGE_LENGTH];
		memset(messageFromServer, 0, MESSAGE_LENGTH);
		bytesRead = 0;
		//int bytesSent = 0;
		bytesRead = read(socketID, messageFromServer, MESSAGE_LENGTH);

		std::cout << "getChat:MESSAGE:bytesRead: " << bytesRead << std::endl;
		std::cout << "getChat:RECEIVED MSG:messageFromAServer: " << messageFromServer << std::endl;
		/* std::vector<std::string> array;
		std::stringstream ss(message);
		std::string tmp; */

		messages.push_back(messageFromServer);
	}
	return messages;
#endif
}

Chat::Chat(std::list<std::string> const &messages)
{
	buffer = messages;
}

void Chat::printChat() // just prints all messages in Chat object
{
	for (auto const &i : buffer)
		std::cout << i << std::endl;
}

std::list<std::string> Chat::listOfMessages()
{
	return buffer;
}