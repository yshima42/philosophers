#include "../includes/philosophers.h"

void	args_check(int ac, char **av)
{
	if ((ac != 5) && (ac != 6))
	{
		printf("arg error\n");
		exit(EXIT_FAILURE);
	}
	(void)av;
}
