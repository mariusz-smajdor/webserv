#pragma once

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_PENDING_CONNECTIONS 512

class Server {
	private:
		sockaddr_in serverAddr;
		sockaddr_in clientAddr;
		socklen_t clientAddrLen;
		int serverFd;
		int clientFd;
		char buffer[BUFFER_SIZE];
		int bytesRead;

	public:
		Server();
		~Server();

		void bindSocket();
		void listenSocket();
		void acceptConnection();
		void readSocket();

		char* getBuffer();

		class ServerException : public std::runtime_error {
			public:
				explicit ServerException(const std::string& message);
		};
};