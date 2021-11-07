#include "../includes/philosophers.h"

void	init_philo(t_conf *conf)
{
	size_t	i;

	conf->philo = my_malloc(sizeof(t_philo *) * conf->num_philos);//freeする
	i = -1;
	while (++i < conf->num_philos)
	{
		conf->philo[i] = my_malloc(sizeof(t_philo));//freeする
		conf->philo[i]->id = i + 1;
		conf->philo[i]->eat_count = 0;
		conf->philo[i]->start_eat_ms = 0;
		conf->philo[i]->has_right_fork = false;
		conf->philo[i]->has_left_fork = false;
		/* conf->philo[i]->status = INVALID;
		pthread_mutex_init(&conf->philo[i]->m_status, NULL); */
		conf->philo[i]->conf = conf;
	}
}

void	init_monitor(t_conf *conf)
{
	size_t	i;

	conf->monitor = my_malloc(sizeof(t_monitor *) * conf->num_philos);//freeする
	i = -1;
	while (++i < conf->num_philos)
	{
		conf->monitor[i] = my_malloc(sizeof(t_monitor));//freeする
		conf->monitor[i]->id = i + 1;
		conf->monitor[i]->philo = conf->philo[i];
		conf->monitor[i]->conf = conf;
	}
}

void	init_forks(t_conf *conf)
{
	size_t	i;

	conf->m_forks = my_malloc(sizeof(pthread_mutex_t *) * conf->num_philos);//freeする
	i = -1;
	while (++i < conf->num_philos)
	{
		conf->m_forks[i] = my_malloc(sizeof(pthread_mutex_t));//freeする
		pthread_mutex_init(conf->m_forks[i], NULL);
	}
}

t_conf	*init_conf(int ac, char **av)
{
	t_conf	*conf;
	
	conf = my_malloc(sizeof(t_conf));//freeする
	conf->num_philos = ft_atoi(av[1]);
	conf->die_ms = ft_atoi(av[2]);
	conf->eat_ms = ft_atoi(av[3]);
	conf->sleep_ms = ft_atoi(av[4]);
	conf->someone_is_dead = false;
	pthread_mutex_init(&conf->m_print, NULL);
	if (ac == 6)
		conf->num_must_eat = ft_atoi(av[5]);
	init_philo(conf);
	init_monitor(conf);
	init_forks(conf);
	return (conf);
}
