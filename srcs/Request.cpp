/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msmajdor <msmajdor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 15:40:35 by msmajdor          #+#    #+#             */
/*   Updated: 2025/02/12 17:26:03 by msmajdor         ###   ########.fr       */
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
			break;
		}
	}
	return 1;
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
	std::cout << _headerBuffer << std::endl;
	return status;
}
