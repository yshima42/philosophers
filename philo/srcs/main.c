/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshimazu <yshimazu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 14:44:08 by yshimazu          #+#    #+#             */
/*   Updated: 2021/11/15 13:05:18 by yshimazu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	print_action(t_conf *conf, size_t id, char *action)
{
	pthread_mutex_lock(&conf->m_print);
	if (conf->someone_is_dead == false)
		printf("%ld %ld %s\n", get_time_ms(), id, action);
	pthread_mutex_unlock(&conf->m_print);
}

bool	is_dead(t_conf *conf, size_t id)
{
	//printf("ms: %zu, sem: %zu, die_ms: %zu", get_time_ms(), conf->philo[id - 1]->last_eat_ms, conf->die_ms);
	if (get_time_ms() - conf->philo[id - 1]->last_eat_ms >= conf->die_ms)
		return (true);
	else
		return (false);
}

int	dead_check(t_philo *philo)
{
	if (philo->conf->someone_is_dead == true)
		return (1);
	if (is_dead(philo->conf, philo->id))
	{
		pthread_mutex_lock(&philo->conf->m_someone_is_dead);
		print_action(philo->conf, philo->id, RED"is dead"END);
		philo->conf->someone_is_dead = true;
		pthread_mutex_unlock(&philo->conf->m_someone_is_dead);
		put_forks(philo->conf, philo->id);
		/* fork_mutex(UNLOCK, RIGHT, philo->id, philo->conf);
		fork_mutex(UNLOCK, LEFT, philo->id, philo->conf); */
		return (1);
	}
	else 
		return (0);
}

int	full_check(t_philo *philo)
{
	usleep(100);
	if (philo->condition == FULL)
	{	
		usleep(philo->conf->eat_ms * 1000);
		return (1);
	}
	else 
		return (0);
}

bool	wait_action_time(t_philo *philo, size_t limit_ms)
{
	size_t	start_ms;
	size_t	current_ms;
	size_t	diff;

	start_ms = get_time_ms();
	while (1)
	{
		current_ms = get_time_ms();
		diff = current_ms - start_ms;
		if (limit_ms <= diff)
			break ;
		if (philo->conf->someone_is_dead == true)
			return (true);
		usleep(500);
	}
	return (false);
}

int	thinking(t_philo *philo)
{
	print_action(philo->conf, philo->id, YELLOW"is thinking"END);	
	if (philo->eat_count == philo->conf->num_must_eat)
		return (1);
	usleep(500);//検討
	return (0);
}

int	sleeping(t_philo *philo)
{
	print_action(philo->conf, philo->id, BLUE"is sleeping"END);
	if (wait_action_time(philo, philo->conf->sleep_ms))
		return (1);
	usleep(50);
	return (0);
}

int	eating(t_philo *philo)
{
	print_action(philo->conf, philo->id, GREEN"is eating"END);
	pthread_mutex_lock(&philo->m_last_eat);
	philo->eat_count++;
	philo->last_eat_ms = get_time_ms();
	pthread_mutex_unlock(&philo->m_last_eat);
	if (wait_action_time(philo, philo->conf->eat_ms))
		return (true);
	if (philo->eat_count == philo->conf->num_must_eat)
		philo->condition = FULL;
	usleep(50);
	return (0);
}

void	*philo_main(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	philo->last_eat_ms = get_time_ms();
	if (philo->id % 2 == 1)
			usleep(philo->conf->eat_ms * 0.9 * 1000);
	while (1)
	{
		if (take_forks(philo->conf, philo->id)
		|| eating(philo)
		|| put_forks(philo->conf, philo->id)
		|| sleeping(philo)
		|| thinking(philo))
			break;
	}
	put_forks(philo->conf, philo->id);//if deadを入れるか検討
	return ("finished");
}

void	*monitor_main(void *arg)
{
	t_monitor	*monitor;

	usleep(100);
	monitor = (t_monitor *)arg;
	while (1)
	{
		if (dead_check(monitor->philo) || full_check(monitor->philo))
			break;
		usleep(1000);
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

int	monitor_create(t_conf *conf)
{
	size_t	i;

	i = -1;
	while (++i < conf->num_philos)
	{
		if (pthread_create(&conf->monitor[i]->thread, NULL, monitor_main, (void *)conf->monitor[i]) != 0)
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
		//printf("id: %zu, thread has finished: %s\n", conf->philo[i]->id, ret);
	}
	return (0);
}

int	monitor_join(t_conf *conf)
{
	size_t	i;

	i = -1;
	while (++i < conf->num_philos)
	{
		char	*ret;
		if (pthread_join(conf->monitor[i]->thread, (void **)&ret) != 0)
		{
			printf ("join error\n");
			return (EXIT_FAILURE);
		}
		//printf("id: %zu, thread has finished: %s\n", conf->monitor[i]->id, ret);
	}
	return (0);
}


void	destroy_all_mutex(t_conf *conf)
{
	destroy_forks(conf);//mutex全部destroyする
	pthread_mutex_destroy(&conf->m_someone_is_dead);
	pthread_mutex_destroy(&conf->m_print);
}

void	free_monitor(t_conf *conf)
{
	size_t			i;

	i = 0;
	while (i < conf->num_philos)
	{
		free(conf->monitor[i]);
		i++;
	}
	free(conf->monitor);
	conf->monitor = NULL;
}

void	free_philo(t_conf *conf)
{
	size_t			i;

	i = 0;
	while (i < conf->num_philos)
	{
		free(conf->philo[i]);
		i++;
	}
	free(conf->philo);
	conf->philo = NULL;
}

void	all_free(t_conf *conf)
{
	free_monitor(conf);
	free_philo(conf);
	free(conf);
}

int	main(int ac, char **av)
{
	t_conf	*conf;

	if (!args_check(ac, av))
		return (EXIT_FAILURE);
	conf = init_conf(ac, av);
	if (philo_create(conf) || monitor_create(conf))
		return (EXIT_FAILURE);
	if (philo_join(conf) ||	monitor_join(conf))
		return (EXIT_FAILURE);
	destroy_all_mutex(conf);
	all_free(conf);
	//printf("end of file\n");
}
