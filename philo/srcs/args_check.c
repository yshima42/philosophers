#include "../includes/philosophers.h"

void	show_usage(void)
{
	printf("Usage:\t./philo num die eat sleep [eat_times]\n\n");
	printf("\tnum:\tnumber_of_philosophers\n");
	printf("\tdie:\ttime_to_die(ms)\n");
	printf("\teat:\ttime_to_eat(ms)\n");
	printf("\tsleep:\ttime_to_sleep(ms)\n");
	printf("\teat_times(option):\n\t\tnumber_of_times_each_philosopher_must_eat\n");
	printf("\n");
}

void	show_num_error(void)
{
	printf("Args need to be positive integer including zero\n");	
}

bool	args_check(int ac, char **av)
{
	if ((ac != 5) && (ac != 6))
	{
		show_usage();
		return (false);
	}
	if (ph_atoi(av[1]) < 0 || ph_atoi(av[2]) < 0 || ph_atoi(av[3]) < 0
		|| ph_atoi(av[4]) < 0)
	{
		show_num_error();
		return (false);
	}
	if (ac == 6 && ph_atoi(av[5]) < 0)
	{
		show_num_error();
		return (false);
	}
	return (true);
}
