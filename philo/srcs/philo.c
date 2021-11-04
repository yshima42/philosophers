#include "../includes/philosophers.h"

void	*philo_main(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(philo->conf->eat_ms * 0.9 * 1000);
	philo->start_eat_ms = gettime_ms();
	change_status(philo, EAT);
	while (1)
	{
		if (take_forks(philo->conf, philo->id)
		|| eating(philo)
		|| put_forks(philo->conf, philo->id)
		|| sleeping(philo)
		|| thinking(philo)
		|| dead_check(philo))
			break;
	}
	return ("finished");
}

int	philo_create(t_conf *conf)
{
	size_t	i;
	
	i = -1;
	while (++i < conf->num_philos)
	{
		if (pthread_create(&conf->philo[i]->thread, NULL, philo_main, (void *)conf->philo[i]) != 0)
		{
			printf("pthread_create error\n");
			return (EXIT_FAILURE);
		}
	}
	return (0);
}

int	philo_join(t_conf *conf)
{
	size_t	i;

	i = -1;
	while (++i < conf->num_philos)
	{
		char		*ret;
		if (pthread_join(conf->philo[i]->thread, (void **)&ret) != 0)
		{
			printf ("join error\n");
			return (EXIT_FAILURE);
		}
		printf("thread has finished: %s\n", ret);
	}
	return (0);
}