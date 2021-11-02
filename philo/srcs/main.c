/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshimazu <yshimazu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 14:44:08 by yshimazu          #+#    #+#             */
/*   Updated: 2021/11/02 18:04:36 by yshimazu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

//eixtを全部消す
// philo1人の時に対処。argで弾く

void	print_action(t_conf *conf, size_t id, char *action)
{
	printf("%ld %ld %s\n", gettime_ms(), id, action);//これで良いのか
	(void)conf;
}

void	take_action(t_philo *philo, size_t limit_ms)
{
	size_t	start_ms;
	size_t	current_ms;
	size_t	diff;

	start_ms = gettime_ms();
	while (1)
	{
		current_ms = gettime_ms();
		diff = current_ms - start_ms;
		if (limit_ms <= diff || philo->status == DEAD)
			break ;//usleepするか検討
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
	if (conf->philo[id - 1]->status == DEAD)
		return (0);
	print_action(conf, id, "has taken a fork");
	ret = fork_mutex(LOCK, LEFT, id, conf);
	if (conf->philo[id - 1]->status == DEAD)
		return (0);
	print_action(conf, id, "has taken a fork");
	return (ret);
}

int put_forks(t_conf *conf, int id)
{
	int	ret;

	ret = fork_mutex(UNLOCK, RIGHT, id, conf);//ここの順番検討
	if (conf->philo[id - 1]->status == DEAD)
		return (0);
	//print_action(conf, id, "has put a RIGHT fork");//消す
	ret = fork_mutex(UNLOCK, LEFT, id, conf);
	if (conf->philo[id - 1]->status == DEAD)
		return (0);
	//print_action(conf, id, "has put a LEFT fork");//消す
	return (ret);
}

int	change_status(t_philo *philo, t_status status)
{
	if (philo->status == DEAD)
		return(0);
	while (0 != pthread_mutex_lock(&philo->m_status))
	{
		printf("%lu: mutex can not lock 4.\n", philo->id);
	}
	philo->status = status;
	
	if (philo->status == SLEEP)
	{
		//printf("time: %lu, die_ms: %lu\n", gettime_ms() - philo->start_eat_ms, philo->conf->die_ms);
		print_action(philo->conf, philo->id, "is sleeping");
	}
	else if (philo->status == THINK)
	{
		//printf("time: %lu, die_ms: %lu\n", gettime_ms() - philo->start_eat_ms, philo->conf->die_ms);
		print_action(philo->conf, philo->id, "is thinking");
	}
	else if (philo->status == DEAD)
	{
		//printf("died time: %lu, die_ms: %lu\n", gettime_ms() - philo->start_eat_ms, philo->conf->die_ms);
		print_action(philo->conf, philo->id, "is dead");
	}
	while (0 != pthread_mutex_unlock(&philo->m_status))
	{
		printf("%lu: mutex can not unlock 4.\n", philo->id);
	}
	return (0);
}

int	eating(t_philo *philo)
{
	int	ret;
	
	if (philo->status == DEAD)
		return (0);
	ret = take_forks(philo->conf, philo->id);
	print_action(philo->conf, philo->id, "is eating");
	philo->start_eat_ms = gettime_ms();
	//printf("before: time: %lu, die_ms: %lu\n", gettime_ms() - philo->start_eat_ms, philo->conf->die_ms);
	take_action(philo, philo->conf->eat_ms);
	//printf("after: time: %lu, die_ms: %lu\n", gettime_ms() - philo->start_eat_ms, philo->conf->die_ms);
	/* philo->eat_count++;//検討する
	if (philo->eat_count == philo->conf->num_must_eat)
		philo->flag_full = true; */
	put_forks(philo->conf, philo->id);
	if (philo->status == DEAD)
		return (ret);
	else
		change_status(philo, SLEEP);
	return (ret);
}

int	sleeping(t_philo *philo)
{
	take_action(philo, philo->conf->sleep_ms);
	if (philo->status == DEAD)
		return (0);
	else
		change_status(philo, THINK);
	return (0);
}

int	thinking(t_philo *philo)
{
	if (philo->status == DEAD)
		return (0);
	else
		change_status(philo, EAT);
	usleep(500);//要検討
	return (0);
}

void	*philo_main(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(philo->conf->eat_ms * 0.9 * 1000);
	philo->start_eat_ms = gettime_ms();
	change_status(philo, EAT);
	while (philo->status != DEAD)
	{
		if (philo->status == EAT)
			eating(philo);
		if (philo->status == SLEEP)
			sleeping(philo);
		if (philo->status == THINK)
			thinking(philo);
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

void	destroy_forks(t_conf *conf)
{
	size_t			i;

	i = 0;
	while (i < conf->num_philos)
	{
		pthread_mutex_destroy(conf->m_forks[i]);
		free(conf->m_forks[i]);
		i++;
	}
	free(conf->m_forks);
	conf->m_forks = NULL;
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

bool	kill_philo(t_conf *conf)
{
	size_t	i;
	i = -1;
	while (++i < conf->num_philos)
	{
		if (conf->philo[i]->status == INVALID)
			return (0);
		if (gettime_ms() - conf->philo[i]->start_eat_ms >= conf->die_ms)
		{
			change_status(conf->philo[i], DEAD);
			return (1);
		}
	}
	return (0);
}

void	finish_threads(t_conf *conf)
{
	size_t	i;

	i = -1;
	while (++i < conf->num_philos)
	{
		printf("change status!!!!\n");
		if (conf->philo[i]->status != DEAD)
		{
			change_status(conf->philo[i], DEAD);
		}
	}
}

bool	full_check(t_conf *conf)
{
	size_t	i;
	size_t	n_full_philo;

	n_full_philo = 0;
	i = -1;
	while (++i < conf->num_philos)
	{
		if (conf->philo[i]->flag_full)
		{
			n_full_philo++;
		}
	}
	if (n_full_philo == conf->num_must_eat)
		return (1);
	else
		return (0);
}

void	*monitor_main(void *arg)
{
	t_conf	*conf;

	conf = (t_conf *)arg;
	while (1)
	{
		if (kill_philo(conf)/*  || full_check(conf) */)
			break;
	}
	finish_threads(conf);
	return ("finish monitor");
}

int	run_monitor(t_conf *conf)
{
	if (pthread_create(&conf->monitor, NULL, monitor_main, (void *)conf) != 0)
	{
		printf("pthread_create error\n");
		return (EXIT_FAILURE);
	}
	if (pthread_join(conf->monitor, NULL) != 0)
	{
		printf("pthread_join error\n");
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
	run_monitor(conf);
	philo_join(conf);
	destroy_forks(conf);
	//pthread_mutex_destroy(&conf->philo[i]->m_status);
	printf("end of file\n");
}