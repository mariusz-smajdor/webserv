/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msmajdor <msmajdor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 18:27:25 by msmajdor          #+#    #+#             */
/*   Updated: 2025/02/09 12:31:15 by msmajdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config(const std::string& configFile)
{
	std::ifstream file(configFile.c_str());

	if (!file.is_open())
	{
		throw ConfigException("Error: Failed to open config file.");
	}
	_parseConfig(file);
}

Config::~Config() {}

void Config::_parseConfig(std::ifstream& file)
{
	std::string line;
	while (std::getline(file, line))
	{
		if (line == "server {")
		{
			_parseServer(file, line);
		}
	}
	file.close();
}

void Config::_parseServer(std::ifstream& file, std::string& line)
{
	ServerConfig server;

	while (std::getline(file, line))
	{
		if (line == "}")
		{
			break;
		}

		size_t i = 0;
		while (i < line.length() && std::isspace(line[i]))
		{
			i++;
		}

		line = line.substr(i);
		if (line.find("host ") != std::string::npos)
		{
			server.host = line.substr(5);
		}
		else if (line.find("port ") != std::string::npos)
		{
			server.port = std::atoi(line.substr(5).c_str());
		}
	}
	_servers.push_back(server);
}

const std::vector<ServerConfig>& Config::getServers() const
{
	return _servers;
}

Config::ConfigException::ConfigException(const std::string& message)
	: _message(message) {}

Config::ConfigException::~ConfigException() throw() {}

const char* Config::ConfigException::what() const throw()
{
	return _message.c_str();
}