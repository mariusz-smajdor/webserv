/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msmajdor <msmajdor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 12:39:50 by msmajdor          #+#    #+#             */
/*   Updated: 2025/02/09 21:46:49 by msmajdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// Later we will get the value from config file
#define MAX_CONNECTIONS 10

class Server
{

private:
	const struct ServerConfig _config;
	int _serverfd;
	int _clientfd;
	int _epollfd;
	struct sockaddr_in _serveraddr;
	struct epoll_event _event;

	int _setNonBlocking(int fd);
	void _handleEpollEvents();
	void _acceptNewClient();
	void _handleExistingClient(int clientfd);

public:
	Server(const ServerConfig& serverConfig);
	~Server();

	class ServerException : public Exception
	{

	public:
		explicit ServerException(const std::string& message);
		virtual ~ServerException() throw();

	};
};
