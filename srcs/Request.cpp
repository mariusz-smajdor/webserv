/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msmajdor <msmajdor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 15:40:35 by msmajdor          #+#    #+#             */
/*   Updated: 2025/02/12 18:20:14 by msmajdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(int clientfd) : _clientfd(clientfd) {}

Request::~Request() {}

// Private methods

int Request::_readHeader()
{
	_totalBytesRead = 0;

	while (_totalBytesRead < BUFFER_SIZE)
	{
		ssize_t bytesRead = recv(_clientfd, _headerBuffer + _totalBytesRead, 1, 0);
		if (bytesRead <= 0)
		{
			return bytesRead;
		}
		_totalBytesRead += bytesRead;
		if (_totalBytesRead >= 4
			&& _headerBuffer[_totalBytesRead - 4] == '\r'
			&& _headerBuffer[_totalBytesRead - 3] == '\n'
			&& _headerBuffer[_totalBytesRead - 2] == '\r'
			&& _headerBuffer[_totalBytesRead - 1] == '\n')
		{
			_headerBuffer[_totalBytesRead] = '\0';
			return bytesRead;
		}
	}
	return -2;
}

void Request::_parseHeader()
{
	int i = 0;

	_parseRequestLine(&i);
}

void Request::_parseRequestLine(int* i)
{
	_method = _extractToken(i);
	_uri = _extractToken(i);
	_httpVersion = _extractToken(i);

	while (std::isspace(_headerBuffer[*i]))
	{
		++(*i);
	}
}

char* Request::_extractToken(int* i)
{
    char* token = &_headerBuffer[*i];

    while (!std::isspace(_headerBuffer[*i]))
	{
        ++(*i);
	}
    _headerBuffer[*i] = '\0';
    ++(*i);
    return token;
}

// Public methods

int Request::readRequest()
{
	(void)_bodyBuffer;
	int status = _readHeader();
	if (status <= 0)
	{
		return status;
	}
	_parseHeader();
	std::cout << "Method: " << _method << std::endl;
	std::cout << "URI: " << _uri << std::endl;
	std::cout << "HTTP Version: " << _httpVersion << std::endl;
	return status;
}
