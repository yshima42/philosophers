/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_destroy.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshimazu <yshimazu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 15:08:07 by yshimazu          #+#    #+#             */
/*   Updated: 2021/11/16 11:12:21 by yshimazu         ###   ########.fr       */
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
		free(conf->m_forks[i]);
		i++;
	}
	free(conf->m_forks);
	conf->m_forks = NULL;
}

void	destroy_all_mutex(t_conf *conf)
{
	destroy_forks(conf);
	pthread_mutex_destroy(&conf->mutex_common);
}

static void	free_monitor(t_conf *conf)
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

static void	free_philo(t_conf *conf)
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
