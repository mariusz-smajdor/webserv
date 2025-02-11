/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msmajdor <msmajdor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 17:41:32 by msmajdor          #+#    #+#             */
/*   Updated: 2025/02/11 18:43:14 by msmajdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int main(int argc, char** av)
{
	try
	{
		Server server(argc > 1 ? av[1] : "config/default.conf");
		server.run();
	}
	catch (const Exception& e)	
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
