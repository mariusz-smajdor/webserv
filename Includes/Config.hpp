/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msmajdor <msmajdor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 18:25:43 by msmajdor          #+#    #+#             */
/*   Updated: 2025/02/09 12:16:39 by msmajdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <exception>

struct ServerConfig
{
	std::string host;
	int port;
};

class Config
{

private:
	std::vector<ServerConfig> _servers;

	void _parseConfig(std::ifstream& file);
	void _parseServer(std::ifstream& file, std::string& line);
public:
	Config(const std::string& configPath);
	~Config();

	const std::vector<ServerConfig>& getServers() const;

	class ConfigException : public std::exception
	{
	
	private:
		std::string _message;

	public:
		explicit ConfigException(const std::string& message);
		virtual ~ConfigException() throw();

		virtual const char* what() const throw();

	};

};
