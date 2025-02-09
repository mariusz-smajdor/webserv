/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msmajdor <msmajdor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 17:06:22 by msmajdor          #+#    #+#             */
/*   Updated: 2025/02/09 17:07:35 by msmajdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <exception>
#include <string>

class Exception : public std::exception
{

protected:
    std::string _message;

public:
    explicit Exception(const std::string& message);
    virtual ~Exception() throw();

    virtual const char* what() const throw();

};
