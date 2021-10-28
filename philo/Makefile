CC				= 	gcc
CFLAGS			= 	-pthread -Wall -Wextra -Werror
NAME			= 	philo
INCLUDES		= 	includes
SRCS_PATH		= 	./srcs/
SRCS_FILES    	= 	main.c ft_atoi.c
SRCS			= 	$(addprefix $(SRCS_PATH)/, $(SRCS_FILES))
SRCS_OBJS		= 	$(SRCS:.c=.o)

all:			$(NAME)

$(NAME):		$(SRCS_OBJS)
				$(CC) $(CFLAGS) $(SRCS_OBJS) -o $(NAME)

.c.o:
				$(CC) $(CFLAGS) -I$(INCLUDES) -c $< -o $@

clean:
				$(RM) $(SRCS_OBJS)

fclean:			clean
				$(RM) $(NAME) $(SRCS_PATH)$(SRCS_OBJS)

re:				fclean all

.PHONY:			all clean fclean re