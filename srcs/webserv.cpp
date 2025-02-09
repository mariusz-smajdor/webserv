/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msmajdor <msmajdor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 17:41:32 by msmajdor          #+#    #+#             */
/*   Updated: 2025/02/09 12:31:28 by msmajdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

int main(int argc, char** av)
{
	try
	{
		Config config(argc > 1 ? av[1] : "config/default.conf");

		const std::vector<ServerConfig>& servers = config.getServers();
		for (size_t i = 0; i < servers.size(); i++)
		{
			std::cout << "Server " << i << " host: " << servers[i].host << " port: " << servers[i].port << std::endl;
		}
	}
	catch (const Config::ConfigException& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
