/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshimazu <yshimazu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 14:44:08 by yshimazu          #+#    #+#             */
/*   Updated: 2021/11/01 17:43:20 by yshimazu         ###   ########.fr       */
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
	conf->dead_flag = 0;
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

int64_t	gettime_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec) * 1000 + (tv.tv_usec) / 1000);
}

void	print_action(t_conf *conf, size_t id, char *action)
{
	printf("%lld %ld %s\n", gettime_ms(), id, action);//これで良いのか確認
	(void)conf;
}

void	take_action(t_philo *philo, int limit_ms)
{
	double	start_ms;
	double	current_ms;
	double	diff;

	start_ms = gettime_ms();
	while (1)
	{
		current_ms = gettime_ms();
		diff = current_ms - start_ms;
		if (limit_ms <= diff || philo->status == DEAD)
			break ;
	}
}

int	fork_mutex(bool is_lock, bool is_right, size_t id, t_conf *conf)
{
	int				ret;
	int				index;

	if (is_right)
		index = id - 1;
	else
	{
		index = id;
		if (id == conf->num_philos)
			index = 0;
	}
	if (is_lock)
		ret = pthread_mutex_lock(conf->m_forks[index]);
	else
		ret = pthread_mutex_unlock(conf->m_forks[index]);
	if (ret != 0)
		printf("%ld: lock, unlock error", id);
	return (ret);
}

int	take_forks(t_conf *conf, size_t id)
{
	int	ret;

	ret = fork_mutex(LOCK, RIGHT, id, conf);
	print_action(conf, id, "has taken a RIGHT fork");
	ret = fork_mutex(LOCK, LEFT, id, conf);
	print_action(conf, id, "has taken a LEFT fork");
	return (ret);
}

int put_forks(t_conf *conf, int id)
{
	int	ret;

	ret = fork_mutex(UNLOCK, RIGHT, id, conf);//ここの順番どうするか検討
	print_action(conf, id, "has put a RIGHT fork");//消す
	ret = fork_mutex(UNLOCK, LEFT, id, conf);
	print_action(conf, id, "has put a LEFT fork");//消す
	return (ret);
}

int	change_status(t_philo *philo, t_status status)
{
	philo->status = status;
	return (0);
}

int	eating(t_philo *philo)
{
	int	ret;
	double	eat_fin_ms;	

	ret = take_forks(philo->conf, philo->id);
	print_action(philo->conf, philo->id, "is eating");
	philo->start_eat_ms = gettime_ms();
	take_action(philo, philo->conf->eat_ms);
	philo->eat_count++;
	put_forks(philo->conf, philo->id);
	eat_fin_ms = gettime_ms() - philo->start_eat_ms;
	printf("id: %ld, eat: %f, die: %zu\n", philo->id, eat_fin_ms, philo->conf->die_ms);
	if ((double)philo->conf->die_ms <= eat_fin_ms || philo->conf->dead_flag == true)
	{
		change_status(philo, DEAD);
		philo->conf->dead_flag = true;
		print_action(philo->conf, philo->id, "is dead while eating\n");
		
	}
	else if (philo->eat_count == philo->conf->num_must_eat)
	{
		change_status(philo, FULL);
		print_action(philo->conf, philo->id, "is full\n");
	}
	else
		change_status(philo, SLEEP);
	return (ret);
}

int	sleeping(t_philo *philo)
{
	double	sleep_fin_ms;

	print_action(philo->conf, philo->id, "is sleeping");
	take_action(philo, philo->conf->sleep_ms);
	sleep_fin_ms = gettime_ms() - philo->start_eat_ms;
	printf("id: %ld, sleep: %f, die: %zu\n", philo->id, sleep_fin_ms, philo->conf->die_ms);
	if (philo->conf->die_ms <= sleep_fin_ms || philo->conf->dead_flag == true)
	{
		change_status(philo, DEAD);
		philo->conf->dead_flag = true;
		print_action(philo->conf, philo->id, "is dead while sleeping\n");
	}
	else
		change_status(philo, THINK);
	return (0);
}

int	thinking(t_philo *philo)
{
	double	think_fin_ms;
	
	print_action(philo->conf, philo->id, "is thinking");
	think_fin_ms = gettime_ms() - philo->start_eat_ms;
	if ((double)philo->conf->die_ms <= think_fin_ms || philo->conf->dead_flag == true)
	{
		change_status(philo, DEAD);
		philo->conf->dead_flag = true;
		print_action(philo->conf, philo->id, "is dead while thinking\n");
	}
	else
		change_status(philo, EAT);
	return (0);
}

void	*philo_main(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(philo->conf->eat_ms * 0.9 * 1000);
	change_status(philo, EAT);
	while (philo->status != DEAD && philo->status != FULL)
	{
		if (philo->status == EAT)
			eating(philo);
		else if (philo->status == SLEEP)
			sleeping(philo);
		else if (philo->status == THINK)
			thinking(philo);
	}
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

void	*monitor_main(void *arg)
{
	t_conf	*conf;
	size_t	i;

	conf = (t_conf *)arg;
	while (1)
	{
		i = -1;
		while (++i == conf->num_philos)
		{
			if (conf->philo[i]->status == DEAD || conf->philo[i]->status == FULL)
				break ;
		}
	}
	return ("monitor finished");
}

int	monitor_run(t_conf *conf)
{
	if (pthread_create(&conf->monitor, NULL, monitor_main, (void *)conf) != 0)
	{
		printf("pthread_create error\n");
		return (EXIT_FAILURE);
	}
	if (pthread_detach(conf->monitor) != 0)
	{
		printf("pthread_create error\n");
		return (EXIT_FAILURE);
	}
	return (0);	
}

int	main(int ac, char **av)
{
	t_conf	*conf;

	args_check(ac, av);
	conf = init_conf(ac, av);
	init_philo(conf);
	init_forks(conf);
	philo_create(conf);
	monitor_run(conf);
	philo_join(conf);
	
	printf("end of file\n");
}