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

void	init_info(int ac, char **av, t_info *info)
{
	info->num_philos = ft_atoi(av[1]);
	info->die_ms = ft_atoi(av[2]);
	info->eat_ms = ft_atoi(av[3]);
	info->sleep_ms = ft_atoi(av[4]);
	if (ac == 6)
		info->num_must_eat = ft_atoi(av[5]);
}

/* void	init_philo(t_info *info)
{
	info->philo = (t_philo **)malloc(t_philo *);
	*(info->philo) = (t_philo *)malloc(t_philo);
} */

void *func(void *arg)
{
	//pthread_mutex_lock();
	int id = *(int *)arg;
	int i = -1;
	printf("%d",*(int *)arg);
	while (++i < 5)
	{
		printf("hello from func: %d\n", id);
		sleep(1);
	}
	return ("finished");
}

int	main(int ac, char **av)
{
	int			i;
	t_info		*info;
	pthread_t	thread[3];
	
	args_check(ac, av);
	info = (t_info *)malloc(sizeof(t_info));//freeする
	if (!info)
	{
		printf("malloc error\n");
		exit(EXIT_FAILURE);
	}
	init_info(ac, av, info);
	/* info->philo = (t_philo **)malloc(sizeof(t_philo));
	*(info->philo) = (t_philo *)malloc(sizeof(t_philo)); */
	i = -1;
	while (++i < 3)
	{
		if (pthread_create(&thread[i], NULL, func, (void *)&i) != 0)
		{
			printf("create error\n");
			exit (EXIT_FAILURE);
		}
	}
	i = -1;
	while (++i < 3)
	{
		char		*ret;
		if (pthread_join(thread[i], (void **)&ret) != 0)
		{
			printf ("join error\n");
			exit(EXIT_FAILURE);
		}
		printf("thread has finished: %s\n", ret);
	}
	//pthread_mutex_destroy(&mutex);
	printf("hello philo\n");
}