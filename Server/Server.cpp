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
	//  "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 2241\n\n<htmlcontent	/>"
	buffer[bytesRead] = '\0';
	// find first space in buffer
	int methodEndIndex = std::string(buffer).find(' ');
	method = std::string(buffer).substr(0, methodEndIndex);

	int requestPathEndIndex = std::string(buffer).find(' ', methodEndIndex + 1);
	requestPath  = std::string(buffer).substr(methodEndIndex + 1, requestPathEndIndex - methodEndIndex - 1);

	int httpVersionEndIndex = std::string(buffer).find('\n', requestPathEndIndex + 1);
	httpVersion = std::string(buffer).substr(requestPathEndIndex + 1, httpVersionEndIndex - requestPathEndIndex - 1);

	message = "";
////////////////////////////hardcoded 200 for now
	if (!method.compare("GET")) {
		//save the file content to message var
		//open file
		std::ifstream file(requestPath.substr(1).c_str());
		if (file.is_open()) {
			std::string line;
			while (std::getline(file, line)) {
				message += line + "\n";
			}
			file.close();
		} else {
			message = "404 Not Found"; // just for now, late will change and save in statusCode variable or sth
		}
		std::cout << message << std::endl;
	// std::cout << "protocol: " << httpVersion << "\n" << message << requestPath << std::endl;
	}
	//                       harcoded                       hardcoded    hardcoded hardcoded
	message = httpVersion + " 200 OK\n" + "Content-Type: text/html\n" + "Content-Length: 9241\n\n" + message;
}

void Server::writeSocket() {
	write(clientFd, message.c_str(), strlen(message.c_str()));
}

// Setters and getters

int Server::getClientFd() {
	return clientFd;
}

Server::ServerException::ServerException(const std::string &message)
    : std::runtime_error(message) {}

