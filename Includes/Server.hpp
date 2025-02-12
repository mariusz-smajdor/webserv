/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msmajdor <msmajdor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 12:39:50 by msmajdor          #+#    #+#             */
/*   Updated: 2025/02/12 17:36:20 by msmajdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"
#include "Request.hpp"
#include <sstream>
#include <cstring>
#include <algorithm>
#include <fcntl.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

// Later we will get the value from config file
#define MAX_EVENTS 10

class Server
{

private:
    Config _config;
    std::vector<int> _serverfds;
    int _epollfd;

	int _setNonBlocking(int fd);
	int _createServerSocket(int port);
	void _handleClientConnection(int serverfd);
	void _handleClientData(int clientfd);

public:
	Server(const std::string& configPath);
	~Server();

	void run();

	class ServerException : public Exception
	{

	public:
		explicit ServerException(const std::string& message);
		virtual ~ServerException() throw();

	};

};
