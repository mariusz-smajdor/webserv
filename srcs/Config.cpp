/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msmajdor <msmajdor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 18:27:25 by msmajdor          #+#    #+#             */
/*   Updated: 2025/02/09 17:17:01 by msmajdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config(const std::string& configFile)
{
	std::ifstream file(configFile.c_str());

	if (!file.is_open())
	{
		throw ConfigException("Failed to open config file.");
	}
	_parseConfig(file);
}

Config::~Config()
{
	if (!_serversConfig.empty())
	{
		_serversConfig.clear();
	}
}

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
	_serversConfig.push_back(server);
}

const std::vector<ServerConfig>& Config::getServersConfig() const
{
	return _serversConfig;
}

// Config Exception

Config::ConfigException::ConfigException(const std::string& message)
	: Exception("Error: Config: " + message) {}

Config::ConfigException::~ConfigException() throw() {}
