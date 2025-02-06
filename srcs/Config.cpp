/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msmajdor <msmajdor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 18:19:41 by msmajdor          #+#    #+#             */
/*   Updated: 2025/02/06 20:30:17 by msmajdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

// Constructor and destructor
Config::Config(const std::string& configFile) : configPath(configFile) {
	parseConfigFile();
}

Config::~Config() {}

// Member functions
bool Config::isValid() const {
	return !servers.empty();
}

void Config::parseConfigFile() {
	std::ifstream file(configPath);
	if (!file.is_open()) {
		std::cerr << "Error: could not open file: " << configPath << std::endl;
		return;
	}

	ServerConfig currentServer;
	std::string line;
	bool inServerBlock = false;
	while (std::getline(file, line)) {
		char* key = strdup(line.c_str());
		char* value = NULL;
		char* token = strtok(key, " \t");

		if (token) {
			value = strtok(NULL, " \t");

			if (strcmp(token), "server") {
				inServerBlock = true;
				currentServer = ServerConfig();
			} else if (strcmp(token, "host") == 0 && inServerBlock) {
				currentServer.host = strdup(value);
			} else if (strcmp(token, "port") == 0 && inServerBlock) {
				currentServer.port = atoi(value);
				if (currentServer.port < MIN_PORT || currentServer.port > MAX_PORT) {
					std::cerr << "Error: invalid port number: " << currentServer.port << std::endl;
					continue;
				}
			} else if (strcmp(token, "}") == 0) {
				if (inServerBlock && currentServer.host) {
					server.push_back(currentServer);
				}
				inServerBlock = false;
			}
		}
		free(key);
	}

	if (servers.empty()) {
		std::cerr << "Error: No valid server configurations found in file: " << configPath << std::endl;
	}
}

// Getters and setters

const std::vector<ServerConfig>& Config::getServers() const {
	return servers;
}
