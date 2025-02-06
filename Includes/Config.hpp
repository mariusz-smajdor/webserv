/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msmajdor <msmajdor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 18:06:43 by msmajdor          #+#    #+#             */
/*   Updated: 2025/02/06 20:26:30 by msmajdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fstream>
#include <string>

#define MIN_PORT 1
#define MAX_PORT 65535
#define DEFAULT_CONFIG_FILE "config/default.conf"

struct ServerConfig {
	char* host;
	int	port;
};

class Config {

private:
	const std::string configPath;
	std::vector<ServerConfig> servers;

	void parseConfigFile();

public:
	// Constructor and destructor
	Config(const std::string& configFile);
	~Config();

	// Member functions
	bool isValid() const;

	// Getters and setters
	const std::vector<ServerConfig>& getServers() const;

};
