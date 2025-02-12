/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msmajdor <msmajdor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 12:55:49 by msmajdor          #+#    #+#             */
/*   Updated: 2025/02/12 17:36:01 by msmajdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(const std::string& configPath)
	: _config(configPath)
{
	_epollfd = epoll_create1(0);
	if (_epollfd == -1)
	{
		throw ServerException("Failed to create epoll instance.");
	}
	
	struct epoll_event event;
	const std::vector<ServerConfig>& servers = _config.getServerConfigs();
	for (size_t i = 0; i < servers.size(); ++i)
	{
		int serverfd = _createServerSocket(servers[i].port);
		_serverfds.push_back(serverfd);
		event.events = EPOLLIN;
		event.data.fd = serverfd;
		if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, serverfd, &event) == -1)
		{
			throw ServerException("Failed to add server socket to epoll instance.");
		}
	}
}

Server::~Server()
{
	close(_epollfd);
	for (size_t i = 0; i < _serverfds.size(); ++i)
	{
		close(_serverfds[i]);
	}
}

// Private methods

int Server::_setNonBlocking(int fd)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
	{
		return -1;
	}
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
	{
		return -1;
	}
	return 0;
}

int Server::_createServerSocket(int port)
{
	int serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverfd == -1)
	{
		throw ServerException("Failed to create server socket.");
	}

	int optval = 1;
	if (setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
	{
		throw ServerException("Failed to set socket options.");
	}

	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = htons(port);
	if (bind(serverfd, (sockaddr*)&serveraddr, sizeof(serveraddr)) == -1)
	{
		std::stringstream ss;
		ss << port;
		throw ServerException("Failed to bind socket on port " + ss.str() + ".");
	}

	if (listen(serverfd, MAX_EVENTS) == -1)
	{
		throw ServerException("Failed to listen on socket.");
	}
	std::cout << "Server listening on port " << port << ".\n";

	_setNonBlocking(serverfd);
	return serverfd;
}

void Server::_handleClientConnection(int serverfd)
{
	struct sockaddr_in clientaddr;
	socklen_t clientaddrlen = sizeof(clientaddr);
	int clientfd = accept(serverfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
	if (clientfd == -1)
	{
		std::cerr << "Failed to accept connection from client.\n";
		return;
	}
	_setNonBlocking(clientfd);
	
	struct epoll_event event;
	event.events = EPOLLIN | EPOLLET;
	event.data.fd = clientfd;
	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, clientfd, &event) == -1)
	{
		std::cerr << "Failed to add client socket to epoll instance.\n";
		// const char* error_message = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
		// send(_clientfd, error_message, strlen(error_message), 0);
		close(clientfd);
	}
}

void Server::_handleClientData(int clientfd)
{
	Request request(clientfd);
	int status = request.readRequest();

	if (status == -1)
	{
		std::cerr << "Failed to read from client socket.\n";
		// const char* error_message = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
		// send(_clientfd, error_message, strlen(error_message), 0);
		close(clientfd);
	}
	else if (status == 0)
	{
		std::cout << "Connection closed by client.\n";
		close(clientfd);
	}
	else
	{
		const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
		send(clientfd, response, strlen(response), 0);
	}
}

// Public methods

void Server::run()
{
	struct epoll_event events[MAX_EVENTS];

	while (true)
	{
		int nfds = epoll_wait(_epollfd, events, MAX_EVENTS, -1);
		if (nfds == -1)
		{
			throw ServerException("Failed to wait for events.");
		}

		for (int i = 0; i < nfds; ++i)
		{
			int fd = events[i].data.fd;
			if (std::find(_serverfds.begin(), _serverfds.end(), fd) != _serverfds.end())
			{
				_handleClientConnection(fd);
			}
			else
			{
				_handleClientData(fd);
			}
		}
	}
}

// Server exception

Server::ServerException::ServerException(const std::string& message)
	: Exception("Error: Server: " + message) {}

Server::ServerException::~ServerException() throw() {}
