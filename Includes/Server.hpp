#pragma once

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_PENDING_CONNECTIONS 512
#define SERVER_MESSAGE "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 2241\n\n<!DOCTYPE html>\n<html lang=\"en\">\n\t<head>\n\t\t<meta charset=\"UTF-8\" />\n\t\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />\n\t\t<title>Document</title>\n\t</head>\n\t<body>\n\t\tsuper html\n\t\t<button>click</button>\n\t</body>\n</html>"

class Server {
	private:
		sockaddr_in serverAddr;
		sockaddr_in clientAddr;
		socklen_t clientAddrLen;
		int serverFd;
		int clientFd;
		char buffer[BUFFER_SIZE];
		int bytesRead;

		std::string method;
		std::string requestPath;
		std::string httpVersion;
		std::string message;

	public:
		Server();
		~Server();

		void bindSocket();
		void listenSocket();
		void acceptConnection();
		void readSocket();
		void writeSocket();

		int getClientFd();

		class ServerException : public std::runtime_error {
			public:
				explicit ServerException(const std::string& message);
		};
};