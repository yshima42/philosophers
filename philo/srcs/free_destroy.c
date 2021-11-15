/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_destroy.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshimazu <yshimazu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 15:08:07 by yshimazu          #+#    #+#             */
/*   Updated: 2021/11/15 15:08:08 by yshimazu         ###   ########.fr       */
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
		pthread_mutex_destroy(&conf->philo[i]->m_last_eat);//要検討　修正
		free(conf->m_forks[i]);
		i++;
	}
	free(conf->m_forks);
	conf->m_forks = NULL;
}

void	destroy_all_mutex(t_conf *conf)
{
	destroy_forks(conf);//mutex全部destroyする
	pthread_mutex_destroy(&conf->m_someone_is_dead);
	pthread_mutex_destroy(&conf->m_everyone_full);
	pthread_mutex_destroy(&conf->m_print);
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
