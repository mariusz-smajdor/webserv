# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msmajdor <msmajdor@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/06 18:06:23 by msmajdor          #+#    #+#              #
#    Updated: 2025/02/06 18:06:35 by msmajdor         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and flags
CXX			:= c++
CXXFLAGS	:= -Wall -Wextra -Werror -std=c++98

# Project structure
NAME		:= webserv
SRCS		:= $(wildcard srcs/*.cpp)
OBJS		:= $(SRCS:.cpp=.o)
INCLUDES	:= -I includes

# Default rule
all: $(NAME)

# Compile the executable
$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

# Compile object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean rules
clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
