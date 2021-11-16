/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_free_destroy.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshimazu <yshimazu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 15:08:07 by yshimazu          #+#    #+#             */
/*   Updated: 2021/11/16 14:52:05 by yshimazu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static void	destroy_forks(t_conf *conf)
{
	size_t			i;

	i = 0;
	while (i < conf->num_philos)
	{
		pthread_mutex_destroy(conf->m_forks[i]);
		if (conf->m_forks[i])
			free(conf->m_forks[i]);
		i++;
	}
	if (conf->m_forks)
		free(conf->m_forks);
	conf->m_forks = NULL;
}

static void	destroy_all_mutex(t_conf *conf)
{
	destroy_forks(conf);
	pthread_mutex_destroy(&conf->m_common);
}

static void	free_monitor(t_conf *conf)
{
	size_t			i;

	i = 0;
	while (i < conf->num_philos)
	{
		if (conf->monitor[i])
			free(conf->monitor[i]);
		i++;
	}
	if (conf->monitor)
		free(conf->monitor);
	conf->monitor = NULL;
}

static void	free_philo(t_conf *conf)
{
	size_t			i;

	i = 0;
	while (i < conf->num_philos)
	{
		if (conf->philo[i])
			free(conf->philo[i]);
		i++;
	}
	if (conf->philo)
		free(conf->philo);
	conf->philo = NULL;
}

void	all_free_destroy(t_conf *conf)
{
	destroy_all_mutex(conf);
	free_monitor(conf);
	free_philo(conf);
	free(conf);
}
