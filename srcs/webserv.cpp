/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msmajdor <msmajdor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 17:41:32 by msmajdor          #+#    #+#             */
/*   Updated: 2025/02/09 17:20:48 by msmajdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int main(int argc, char** av)
{
	try
	{
		Config config(argc > 1 ? av[1] : "config/default.conf");

		const std::vector<ServerConfig>& serversConfig = config.getServersConfig();
		for (size_t i = 0; i < serversConfig.size(); i++)
		{
			Server server(serversConfig[i]);
		}
	}
	catch (const Exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
