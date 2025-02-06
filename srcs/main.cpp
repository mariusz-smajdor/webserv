#include "Server/Server.hpp"

int main() {
	try {
		Server server;
		server.bindSocket();
		server.listenSocket();
		while (true) {
			server.acceptConnection();
			server.readSocket();
			server.writeSocket();
			close(server.getClientFd());
		}
	} catch (const Server::ServerException &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
}