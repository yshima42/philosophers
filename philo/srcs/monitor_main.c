/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshimazu <yshimazu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 15:09:03 by yshimazu          #+#    #+#             */
/*   Updated: 2021/11/15 15:10:01 by yshimazu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static int	dead_check(t_philo *philo)
{
	if (philo->conf->someone_is_dead == true)
		return (1);
	if (get_time_ms() - philo->last_eat_ms >= philo->conf->die_ms)
	{
		pthread_mutex_lock(&philo->conf->m_someone_is_dead);
		print_action(philo->conf, philo->id, RED"is dead"END);
		philo->conf->someone_is_dead = true;
		pthread_mutex_unlock(&philo->conf->m_someone_is_dead);
		put_forks(philo->conf, philo->id);
		return (1);
	}
	else
		return (0);
}

static int	full_check(t_philo *philo)
{
	size_t	i;
	size_t	num_full_philos;

	usleep(100);
	if (philo->conf->everyone_full == true)
		return (1);
	i = 0;
	num_full_philos = 0;
	while (i < philo->conf->num_philos)
	{
		if (philo->conf->philo[i]->condition == FULL)
			num_full_philos++;
		i++;
	}
	if (num_full_philos == philo->conf->num_philos)
	{
		pthread_mutex_lock(&philo->conf->m_everyone_full);
		philo->conf->everyone_full = true;
		pthread_mutex_unlock(&philo->conf->m_everyone_full);
		return (1);
	}
	else
		return (0);
}

void	*monitor_main(void *arg)
{
	t_monitor	*monitor;

	usleep(100);
	monitor = (t_monitor *)arg;
	while (1)
	{
		if (dead_check(monitor->philo) || full_check(monitor->philo))
			break ;
		usleep(1000);
	}
	return ("finished");
}
