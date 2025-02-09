/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msmajdor <msmajdor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 18:25:43 by msmajdor          #+#    #+#             */
/*   Updated: 2025/02/09 17:19:25 by msmajdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Exception.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>

struct ServerConfig
{
	std::string	host;
	int	port;
};

class Config
{

private:
	std::vector<ServerConfig> _serversConfig;

	void _parseConfig(std::ifstream& file);
	void _parseServer(std::ifstream& file, std::string& line);
public:
	Config(const std::string& configPath);
	~Config();

	const std::vector<ServerConfig>& getServersConfig() const;

	class ConfigException : public Exception
	{

	public:
		explicit ConfigException(const std::string& message);
		virtual ~ConfigException() throw();

	};

};
