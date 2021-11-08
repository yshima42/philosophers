/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshimazu <yshimazu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 14:44:08 by yshimazu          #+#    #+#             */
/*   Updated: 2021/11/08 12:29:40 by yshimazu         ###   ########.fr       */
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
	//printf("ms: %zu, sem: %zu, die_ms: %zu", get_time_ms(), conf->philo[id - 1]->start_eat_ms, conf->die_ms);
	if (get_time_ms() - conf->philo[id - 1]->start_eat_ms >= conf->die_ms)
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
		print_action(philo->conf, philo->id, RED"is dead"END);
		philo->conf->someone_is_dead = true;
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
		//print_action(philo->conf, philo->id, RED"is full"END);
		//philo->conf->someone_is_dead = true;
		//put_forks(philo->conf, philo->id);
		/* fork_mutex(UNLOCK, RIGHT, philo->id, philo->conf);
		fork_mutex(UNLOCK, LEFT, philo->id, philo->conf); */
		return (1);
	}
	else 
		return (0);
}

size_t	start_time_set(t_philo *philo)
{
	size_t	start_time_ms;
	
	if (philo->conf->num_philos % 2 == 0)
	{
		if (philo->id % 2 != 0)
			start_time_ms = philo->conf->eat_ms;
		else
			start_time_ms = 10;//検討
	}
	else
	{
		if (philo->id == 1)
			start_time_ms = philo->conf->eat_ms * 2;
		else if (philo->id % 2 != 0)
			start_time_ms = 10;
		else
			start_time_ms = philo->conf->eat_ms;
	}
	return (start_time_ms);
}

bool	wait_action_time(t_philo *philo, size_t limit_ms)
{
	size_t	start_ms;
	size_t	current_ms;
	size_t	diff;

	(void)philo;//必要なければ最後に消す
	start_ms = get_time_ms();
	while (1)
	{
		current_ms = get_time_ms();
		diff = current_ms - start_ms;
		if (limit_ms <= diff)
			break ;
		if (philo->conf->someone_is_dead == true)
			return (true);
		/* if (dead_check(philo))
			return (true); */
		usleep(500);
	}
	return (false);
}

int	thinking(t_philo *philo)
{
	print_action(philo->conf, philo->id, YELLOW"is thinking"END);	
	/* if (philo->conf->num_philos % 2 == 1)
		if (wait_action_time(philo, philo->conf->eat_ms))
		return (1); */
	//wait_action_time(philo, start_time_set(philo));
	//usleep(100);//検討
	return (0);
}

int	sleeping(t_philo *philo)
{
	print_action(philo->conf, philo->id, BLUE"is sleeping"END);
	if (wait_action_time(philo, philo->conf->sleep_ms))
		return (true);
	//usleep(50);
	return (0);
}

int	eating(t_philo *philo)
{
	print_action(philo->conf, philo->id, GREEN"is eating"END);
	philo->eat_count++;
	philo->start_eat_ms = get_time_ms();
	//printf("id: %zu, philo->start_eat_ms: %zu\n", philo->id, philo->start_eat_ms);
	if (wait_action_time(philo, philo->conf->eat_ms))
		return (true);
	if (philo->eat_count == philo->conf->num_must_eat)
	{
		print_action(philo->conf, philo->id, RED"is full"END);
		put_forks(philo->conf, philo->id);
		//philo->condition = FULL;
		//usleep(1000);
		return (1);
	}
	usleep(50);
	return (0);
}

void	*philo_main(void *arg)
{
	t_philo	*philo;
	size_t	start_time_ms;

	philo = (t_philo *)arg;
	start_time_ms = start_time_set(philo);
	//printf("id: %zu, fsm: %zu\n",philo->id, start_time_ms);
	philo->start_eat_ms = get_time_ms();
	wait_action_time(philo, start_time_ms);
	//printf("id: %zu, now: %zu\n",philo->id, get_time_ms());
	while (1)
	{
		if (take_forks(philo->conf, philo->id)
		|| eating(philo)
		|| put_forks(philo->conf, philo->id)
		|| sleeping(philo)
		|| thinking(philo))
			break;
	}
	//if (philo->has_right_fork)
		fork_mutex(UNLOCK, RIGHT, philo->id, philo->conf);
	//if (philo->has_left_fork) //入れるかどうか検討
		fork_mutex(UNLOCK, LEFT, philo->id, philo->conf);
	return ("finished");
}

void	*monitor_main(void *arg)
{
	t_monitor	*monitor;

	usleep(100);
	monitor = (t_monitor *)arg;
	//printf("id: %zu, now: %zu\n",philo->id, get_time_ms());	
	while (1)
	{
		if (dead_check(monitor->philo) || full_check(monitor->philo))
		{
			//printf("ms: %zu\n",get_time_ms());
			//printf("moni-id:%zu, philo-id: %zu\n", monitor->id, monitor->philo->id);
			break;
		}
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

	args_check(ac, av);
	conf = init_conf(ac, av);
	if (philo_create(conf) || monitor_create(conf))
		return (EXIT_FAILURE);
	if (philo_join(conf) ||	monitor_join(conf))
		return (EXIT_FAILURE);
	destroy_all_mutex(conf);
	all_free(conf);
	printf("end of file\n");
}
