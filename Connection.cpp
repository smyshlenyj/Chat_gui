#include "Connection.h"

Connection::Connection() = default;
Connection::~Connection() = default;

std::string serverAddress = "192.168.186.128";

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
SOCKET Connection::connectClientOpen()
{

	int iResult;
	WSADATA wsaData;

	SOCKET ConnectSocket = INVALID_SOCKET;
	struct sockaddr_in clientService;
	char recvbuf[MESSAGE_LENGTH] = "";
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		wprintf(L"WSAStartup failed with error: %d\n", iResult);
		return 1;
	}
	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ConnectSocket == INVALID_SOCKET) {
		wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr(serverAddress.c_str());
	clientService.sin_port = htons(PORT);

	iResult = connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService));
	if (iResult == SOCKET_ERROR) {
		wprintf(L"connect failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	return ConnectSocket;
}
#else
int Connection::connectClientOpen()
{
	struct sockaddr_in serveraddress;

	int socket_discriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_discriptor == -1)
	{
		std::cout << "Error creating socket!" << std::endl;
		return -1;
	}

	serveraddress.sin_family = AF_INET;
	serveraddress.sin_port = htons(PORT);
	serveraddress.sin_addr.s_addr = inet_addr(serverAddress.c_str());

	int defaultConnection = connect(socket_discriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
	if (defaultConnection == -1)
	{
		std::cout << "Unable to connect to server" << std::endl;
		return -1;
	}

	return socket_discriptor;
}
#endif

void Connection::connectClose(int socket_discriptor)
{
	std::string usr = "TerminateSession";
	char msg[MESSAGE_LENGTH];
	memset(msg, 0, MESSAGE_LENGTH);
	strcpy(msg, usr.c_str());

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	send(socket_discriptor, msg, MESSAGE_LENGTH, NULL);
	closesocket(socket_discriptor);
#endif
std::cout << "TerminateSession sent to server " << std::endl;
#ifdef __linux__
	write(socket_discriptor, msg, MESSAGE_LENGTH);
	close(socket_discriptor);
#endif
}