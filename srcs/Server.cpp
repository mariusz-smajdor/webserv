/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msmajdor <msmajdor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 12:55:49 by msmajdor          #+#    #+#             */
/*   Updated: 2025/02/09 21:47:36 by msmajdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(const struct ServerConfig& config)
	: _config(config), _serverfd(-1), _clientfd(-1), _epollfd(-1)
{
	if ((_serverfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		throw ServerException("Failed to create server socket.");
	}
	if (_setNonBlocking(_serverfd) == -1)
	{
		throw ServerException("Failed to set server socket to non-blocking mode.");
	}

    memset(&_serveraddr, 0, sizeof(_serveraddr));
    _serveraddr.sin_family = AF_INET;
    _serveraddr.sin_addr.s_addr = INADDR_ANY;
    _serveraddr.sin_port = htons(_config.port);
	if (bind(_serverfd, (struct sockaddr*)&_serveraddr, sizeof(_serveraddr)) == -1)
	{
		std::stringstream ss;
		ss << _config.port;
		throw ServerException("Failed to bind socket on port " + ss.str() + ".");
	}

	if (listen(_serverfd, MAX_CONNECTIONS) == -1)
	{
		throw ServerException("Failed to listen on socket.");
	}

	if ((_epollfd = epoll_create1(0)) == -1)
	{
		throw ServerException("Failed to create epoll instance.");
	}

	_event.events = EPOLLIN;
	_event.data.fd = _serverfd;
	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, _serverfd, &_event) == -1)
	{
		throw ServerException("Failed to add server socket to epoll instance.");
	}

	_handleEpollEvents();
}

Server::~Server()
{
	if (_serverfd != -1)
	{
		close(_serverfd);
	}
	if (_clientfd != -1)
	{
		close(_clientfd);
	}
	if (_epollfd != -1)
	{
		close(_epollfd);
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

void Server::_handleEpollEvents()
{
	struct epoll_event events[MAX_CONNECTIONS];

	while (true)
	{
		int nfds = epoll_wait(_epollfd, events, MAX_CONNECTIONS, -1);
		if (nfds == -1)
		{
			throw ServerException("Failed to wait for events.");
		}

		for (int i = 0; i < nfds; i++)
		{
			if (events[i].data.fd == _serverfd)
			{
				_acceptNewClient();
			}
			else
			{
				_handleExistingClient(events[i].data.fd);
			}
		}
	}
}

void Server::_acceptNewClient()
{
	if ((_clientfd = accept(_serverfd, NULL, NULL)) == -1)
	{
		throw ServerException("Failed to accept connection.");
	}
	
	if (_setNonBlocking(_clientfd) == -1)
	{
		std::cerr << "Failed to set client socket to non-blocking mode.\n";
		// const char* error_message = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/plain\r\n\r\nFailed to set socket to non-blocking mode.";
		// send(client_fd, error_message, strlen(error_message), 0);
		close(_clientfd);
		return;
	}

	_event.events = EPOLLIN | EPOLLET;
	_event.data.fd = _clientfd;
	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, _clientfd, &_event) == -1)
	{
		std::cerr << "Failed to add client socket to epoll instance.\n";
		// const char* error_message = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
		// send(_clientfd, error_message, strlen(error_message), 0);
		close(_clientfd);
	}
}

void Server::_handleExistingClient(int clientfd)
{
	char buffer[1024];
	int bytes_read = read(clientfd, buffer, sizeof(buffer));

	if (bytes_read == -1)
	{
		std::cerr << "Failed to read data from client.\n";
		// const char* error_response = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
		// send(clientfd, error_response, strlen(error_response), 0);
		close(clientfd);
	}
	else if (bytes_read == 0)
	{
		std::cout << "Connection closed by client.\n";
		close(clientfd);
	}
	else
	{
		std::cout << "Received data:\n" << std::string(buffer, bytes_read) << "\n";
		const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
		send(clientfd, response, strlen(response), 0);
	}
}

// Server exception

Server::ServerException::ServerException(const std::string& message)
	: Exception("Error: Server: " + message) {}

Server::ServerException::~ServerException() throw() {}
