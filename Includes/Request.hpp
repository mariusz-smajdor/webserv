/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msmajdor <msmajdor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:47:11 by msmajdor          #+#    #+#             */
/*   Updated: 2025/02/14 20:27:59 by msmajdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>

#define BUFFER_SIZE 8192

class Request
{

private:
	int _clientfd;
	size_t _totalBytesRead;
	char _headerBuffer[BUFFER_SIZE];
	char _bodyBuffer[BUFFER_SIZE];
	char* _method;
	char* _uri;
	char* _httpVersion;
	std::vector<std::pair<char*, char*> > _headers;

	int _readHeader();
	void _parseRequestLine(int* i);
	void _parseHeaders(int* i);
	char* _extractToken(int* i, char delim = ' ', int step = 1);
	int _readBody(int contentLength);

public:
	Request(int clientfd);
	~Request();

	int readRequest();

};
