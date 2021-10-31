/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshimazu <yshimazu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 14:44:08 by yshimazu          #+#    #+#             */
/*   Updated: 2021/10/31 11:28:46 by yshimazu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

//eixtを全部消す

void	args_check(int ac, char **av)
{
	if ((ac != 5) && (ac != 6))
	{
		printf("arg error\n");
		exit(EXIT_FAILURE);
	}
	(void)av;
}

void	*my_malloc(size_t size)
{
	void	*ptr;
	ptr = malloc(size);
	if (!ptr)
	{
		ft_putstr_fd("malloc error\n", 2);
		exit(EXIT_FAILURE);
	}
	return (ptr);
}

t_conf	*init_conf(int ac, char **av)
{
	t_conf	*conf;
	
	conf = my_malloc(sizeof(t_conf));//freeする
	conf->num_philos = ft_atoi(av[1]);
	conf->die_ms = ft_atoi(av[2]);
	conf->eat_ms = ft_atoi(av[3]);
	conf->sleep_ms = ft_atoi(av[4]);
	if (ac == 6)
		conf->num_must_eat = ft_atoi(av[5]);
	return (conf);
}

void	print_conf(t_conf *conf)
{
	printf("[print_conf]\n");
	printf("conf->num_philos: %lu\n", conf->num_philos);
	printf("conf->die_ms: %lu\n", conf->die_ms);
	printf("conf->eat_ms: %lu\n", conf->eat_ms);
	printf("conf->sleap_ms: %lu\n", conf->sleep_ms);
	printf("conf->num_must_eat: %lu\n", conf->num_must_eat);
	printf("\n");
}

void	print_philo(t_philo **philo)
{
	size_t	i;
	
	i = -1;
	while (++i < philo[1]->conf->num_philos)
	{
		printf("[print_philo]\n");
		printf("id: %lu\n", philo[i]->id);
		printf("status: %u\n", philo[i]->status);
		printf("\n");
		print_conf(philo[i]->conf);
	}
}

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
		conf->philo[i]->status = INVALID;
		conf->philo[i]->conf = conf;
	}
}

int	fork_mutex(bool is_lock, bool is_right, size_t id, t_conf *conf)
{
	int				ret;
	int				index;

	if (is_right)
	{
		index = id;
		if (id == conf->num_philos)
			index = 0;
	}
	else
		index = id - 1;
	if (is_lock)
		ret = pthread_mutex_lock(conf->m_forks[index]);
	else
		ret = pthread_mutex_unlock(conf->m_forks[index]);
	if (ret != 0)
	{
		printf("%ld: mutex can not (un)lock\n", id);
	}
	return (ret);
}

int	take_forks(t_conf *conf, size_t id)
{
	int	ret;

	ret = fork_mutex(LOCK, RIGHT, id, conf);
	printf("has taken a RIGHT fork\n");
	ret = fork_mutex(LOCK, LEFT, id, conf);
	printf("has taken a LEFT fork\n");

	return (ret);
}

int put_forks(t_conf *conf, int id)
{
	int	ret;

	ret = fork_mutex(UNLOCK, RIGHT, id, conf);//ここの順番どうするか検討
	printf("has put a RIGHT fork\n");
	ret = fork_mutex(UNLOCK, LEFT, id, conf);
	printf("has put a LEFT fork\n");
	return (ret);
}

void	*philo_main(void *arg)
{
	t_philo	*philo;
	struct timeval	tv;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 1)
		usleep(200);
	gettimeofday(&tv, NULL);
	printf("%06d ", tv.tv_usec);
	printf("%lu ", philo->id);
	take_forks(philo->conf, philo->id);
	put_forks(philo->conf, philo->id);
	
	return ("finished");
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

int	main(int ac, char **av)
{
	size_t	i;
	t_conf	*conf;

	args_check(ac, av);
	conf = init_conf(ac, av);
	init_philo(conf);
	init_forks(conf);
	i = -1;
	while (++i < conf->num_philos)
	{
		if (pthread_create(&conf->philo[i]->thread, NULL, philo_main, (void *)conf->philo[i]) != 0)
		{
			printf("pthread_create error\n");
			return (EXIT_FAILURE);
		}
	}
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
	printf("end of file\n");
}