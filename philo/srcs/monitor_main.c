/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshimazu <yshimazu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 15:09:03 by yshimazu          #+#    #+#             */
/*   Updated: 2021/11/16 10:47:19 by yshimazu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static int	dead_check(t_philo *philo)
{
	if (finish_check(philo->conf))
		return (1);
	pthread_mutex_lock(&philo->conf->m_common);
	if (get_time_ms() - philo->last_eat_ms >= philo->conf->die_ms)
	{
		printf("%ld %ld %s\n", get_time_ms(), philo->id, RED"is dead"END);
		philo->conf->finish_flag = true;
		pthread_mutex_unlock(&philo->conf->m_common);
		return (1);
	}
	else
	{
		pthread_mutex_unlock(&philo->conf->m_common);
		return (0);
	}
}

void	*monitor_main(void *arg)
{
	t_monitor	*monitor;

	usleep(100);
	monitor = (t_monitor *)arg;
	while (1)
	{
		if (dead_check(monitor->philo))
			break ;
		usleep(1000);
	}
	return ("finished");
}