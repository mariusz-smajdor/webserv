NAME	= webserv
RM		= rm -f

CPP		= c++
CFLAGS	= -Wall -Wextra -Werror -std=c++98

SRCS	= webserv.cpp Server/Server.cpp
OBJS	= $(SRCS:.cpp=.o)

all: $(NAME)

%.o: %.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CPP) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re



