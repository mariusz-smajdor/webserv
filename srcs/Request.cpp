/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msmajdor <msmajdor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 15:40:35 by msmajdor          #+#    #+#             */
/*   Updated: 2025/02/14 20:36:18 by msmajdor         ###   ########.fr       */
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
		ssize_t bytesRead = recv(_clientfd, _headerBuffer + _totalBytesRead, 1, MSG_DONTWAIT);

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

int Request::_readBody(int contentLength)
{
	ssize_t bytesRead = recv(_clientfd, _bodyBuffer, contentLength, MSG_DONTWAIT);

	if (bytesRead <= 0)
	{
		return bytesRead;
	}
	_bodyBuffer[bytesRead] = '\0';
	return bytesRead;
}

// Public methods

int Request::readRequest()
{
	int status = _readHeader();
	if (status <= 0)
	{
		return status;
	}

	int i = 0;
	_parseRequestLine(&i);
	_parseHeaders(&i);

	int contentLength = 0;
	for (size_t i = 0; i < _headers.size(); i++)
	{
		if (std::strcmp(_headers[i].first, "content-length") == 0)
		{
			contentLength = std::atoi(_headers[i].second);
			if (contentLength >= BUFFER_SIZE)
			{
				return -2;
			}
			status = _readBody(contentLength);
			if (status <= 0)
			{
				return status;
			}
		}
	}

	std::cout << "Method: " << _method << std::endl;
	std::cout << "URI: " << _uri << std::endl;
	std::cout << "HTTP Version: " << _httpVersion << std::endl;
	for (size_t i = 0; i < _headers.size(); ++i)
	{
		std::cout << _headers[i].first << ": " << _headers[i].second << std::endl;
	}
	std::cout << "Body:" << _bodyBuffer << "\n";
	std::cout << std::endl;
	return status;
}
