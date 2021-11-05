#include "../includes/philosophers.h"

size_t	first_sleep_check(t_philo *philo)
{
	size_t	first_sleep_ms;
	
	if (philo->conf->num_philos % 2 == 0)
	{
		if (philo->id % 2 != 0)
			first_sleep_ms = philo->conf->eat_ms;
		else
			first_sleep_ms = 0;
	}
	else
	{
		if (philo->id % 3 == 0)
			first_sleep_ms = philo->conf->eat_ms * 2;
		else if (philo->id % 3 == 1)
			first_sleep_ms = 0;
		else
			first_sleep_ms = philo->conf->eat_ms;
	}
	return (first_sleep_ms);
}

void	*philo_main(void *arg)
{
	t_philo	*philo;
	size_t	first_sleep_ms;

	philo = (t_philo *)arg;
	first_sleep_ms = first_sleep_check(philo);
	printf("id: %zu, fsm: %zu\n",philo->id, first_sleep_ms);
	philo->start_eat_ms = gettime_ms();
	if (take_action(philo, first_sleep_ms))
		return ("finished");
	change_status(philo, EAT);
	while (1)
	{
		if (take_forks(philo->conf, philo->id)
		|| eating(philo)
		|| put_forks(philo->conf, philo->id)
		|| sleeping(philo)
		|| thinking(philo))
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
