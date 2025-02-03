#include "Server/Server.hpp"

int main() {
	try {
		Server server;
		server.bindSocket();
		server.listenSocket();
		server.acceptConnection();
		server.readSocket();
		std::cout << "Received: " << server.getBuffer() << std::endl;
	} catch (const Server::ServerException &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
    return 0;
}