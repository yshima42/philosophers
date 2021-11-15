CC				= 	gcc
CFLAGS			= 	-pthread -Wall -Wextra -Werror -fsanitize=thread
NAME			= 	philo
INCLUDES		= 	includes
SRCS_PATH		= 	./srcs/
SRCS_FILES    	= 	main.c ph_atoi.c libft.c args_check.c init.c utils.c fork.c
SRCS			= 	$(addprefix $(SRCS_PATH), $(SRCS_FILES))
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