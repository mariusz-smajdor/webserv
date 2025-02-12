/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msmajdor <msmajdor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 14:47:11 by msmajdor          #+#    #+#             */
/*   Updated: 2025/02/12 18:17:58 by msmajdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
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

	int _readHeader();
	void _parseHeader();
	void _parseRequestLine(int* i);
	char* _extractToken(int* i);

public:
	Request(int clientfd);
	~Request();

	int readRequest();

};