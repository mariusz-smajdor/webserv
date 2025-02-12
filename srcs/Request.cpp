/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msmajdor <msmajdor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 15:40:35 by msmajdor          #+#    #+#             */
/*   Updated: 2025/02/12 19:30:03 by msmajdor         ###   ########.fr       */
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

void Request::_parseRequestLine(int* i)
{
	_method = _extractToken(i);
	_uri = _extractToken(i);
	_httpVersion = _extractToken(i, '\r', 2);

	while (std::isspace(_headerBuffer[*i]))
	{
		++(*i);
	}
}

void Request::_parseHeaders(int* i)
{
	while (_headerBuffer[*i] != '\r')
	{
		char* key = _extractToken(i, ':', 2);
		char* value = _extractToken(i, '\r', 2);
		_headers.push_back(std::make_pair(key, value));
	}
}

char* Request::_extractToken(int* i, char delimiter, int step)
{
    char* token = &_headerBuffer[*i];

    while (_headerBuffer[*i] != delimiter)
	{
        ++(*i);
	}
    _headerBuffer[*i] = '\0';
    *i += step;
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

	int i = 0;
	_parseRequestLine(&i);
	_parseHeaders(&i);
	std::cout << "Method: " << _method << std::endl;
	std::cout << "URI: " << _uri << std::endl;
	std::cout << "HTTP Version: " << _httpVersion << std::endl;
	for (size_t i = 0; i < _headers.size(); ++i)
	{
		std::cout << _headers[i].first << ": " << _headers[i].second << std::endl;
	}
	std::cout << std::endl;
	return status;
}
