/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msmajdor <msmajdor@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 17:09:43 by msmajdor          #+#    #+#             */
/*   Updated: 2025/02/09 17:10:06 by msmajdor         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Exception.hpp"

Exception::Exception(const std::string& message)
    : _message(message) {}

Exception::~Exception() throw() {}

const char* Exception::what() const throw()
{
    return _message.c_str();
}
