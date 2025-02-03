#include "Server.hpp"

Server::Server() {
	serverFd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (serverFd == -1) {
    	throw Server::ServerException("Socket creation failed. errno: " + std::string(std::strerror(errno)));
	}
}

Server::~Server() {
    if (serverFd != -1) {
        close(serverFd);
    }
}

void Server::bindSocket() {
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(serverFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        throw Server::ServerException("Failed to bind to port. errno: " + std::string(std::strerror(errno)));
    }
}

void Server::listenSocket() {
	if (listen(serverFd, MAX_PENDING_CONNECTIONS) < 0) {
		throw Server::ServerException("Failed to listen on socket. errno: " + std::string(std::strerror(errno)));
	}
}

void Server::acceptConnection() {
	clientAddrLen = sizeof(clientAddr);
	clientFd = accept(serverFd, (sockaddr*)&clientAddr, &clientAddrLen);

	if (clientFd < 0) {
		throw Server::ServerException("Failed to grab connection. errno: " + std::string(std::strerror(errno)));
	}
}

void Server::readSocket() {
	bytesRead = read(clientFd, buffer, BUFFER_SIZE - 1);
	buffer[bytesRead] = '\0';
}

char* Server::getBuffer() {
	return buffer;
}

Server::ServerException::ServerException(const std::string &message)
    : std::runtime_error(message) {}
