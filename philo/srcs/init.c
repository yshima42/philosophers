/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshimazu <yshimazu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 15:08:12 by yshimazu          #+#    #+#             */
/*   Updated: 2021/11/16 21:11:24 by yshimazu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static void	init_philo(t_conf *conf)
{
	size_t	i;

	conf->philo = my_malloc(sizeof(t_philo *) * conf->num_philos);
	i = -1;
	while (++i < conf->num_philos)
	{
		conf->philo[i] = my_malloc(sizeof(t_philo));
		conf->philo[i]->id = i + 1;
		conf->philo[i]->eat_count = 0;
		conf->philo[i]->last_eat_ms = 0;
		conf->philo[i]->has_right_fork = false;
		conf->philo[i]->has_left_fork = false;
		conf->philo[i]->conf = conf;
	}
}

static void	init_monitor(t_conf *conf)
{
	size_t	i;

	conf->monitor = my_malloc(sizeof(t_monitor *) * conf->num_philos);
	i = -1;
	while (++i < conf->num_philos)
	{
		conf->monitor[i] = my_malloc(sizeof(t_monitor));
		conf->monitor[i]->id = i + 1;
		conf->monitor[i]->philo = conf->philo[i];
		conf->monitor[i]->conf = conf;
	}
}

static void	init_forks(t_conf *conf)
{
	size_t	i;

	conf->m_forks = my_malloc(sizeof(pthread_mutex_t *) * conf->num_philos);
	i = -1;
	while (++i < conf->num_philos)
	{
		conf->m_forks[i] = my_malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(conf->m_forks[i], NULL);
	}
}

t_conf	*init_conf(int ac, char **av)
{
	t_conf	*conf;

	conf = my_malloc(sizeof(t_conf));
	conf->num_philos = ph_atoi(av[1]);
	conf->die_ms = ph_atoi(av[2]);
	conf->eat_ms = ph_atoi(av[3]);
	conf->sleep_ms = ph_atoi(av[4]);
	if (ac == 6)
		conf->num_must_eat = ph_atoi(av[5]);
	else
		conf->num_must_eat = 0;	
	conf->num_full_philos = 0;
	conf->finish_flag = false;
	pthread_mutex_init(&conf->m_common, NULL);
	init_philo(conf);
	init_monitor(conf);
	init_forks(conf);
	return (conf);
}
