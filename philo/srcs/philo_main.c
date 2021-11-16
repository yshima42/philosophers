/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshimazu <yshimazu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 15:10:39 by yshimazu          #+#    #+#             */
/*   Updated: 2021/11/16 11:01:36 by yshimazu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

bool	finish_check(t_conf	*conf)
{
	pthread_mutex_lock(&conf->m_common);
	if (conf->finish_flag == true)
	{
		pthread_mutex_unlock(&conf->m_common);
		return (true);
	}
	pthread_mutex_unlock(&conf->m_common);
	return (false);
}

static bool	wait_action_time(t_philo *philo, size_t limit_ms)
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
		if (finish_check(philo->conf))
			return (true);
		usleep(500);
	}
	return (false);
}

static int	thinking(t_philo *philo)
{
	print_action(philo->conf, philo->id, GREEN"is thinking"END);
	usleep(500);//検討
	return (0);
}

static int	sleeping(t_philo *philo)
{
	print_action(philo->conf, philo->id, BLUE"is sleeping"END);
	if (wait_action_time(philo, philo->conf->sleep_ms))
		return (1);
	return (0);
}

static int	eating(t_philo *philo)
{
	print_action(philo->conf, philo->id, YELLOW"is eating"END);
	pthread_mutex_lock(&philo->conf->m_common);
	philo->eat_count++;
	philo->last_eat_ms = get_time_ms();	
	if (philo->eat_count == philo->conf->num_must_eat)
		philo->conf->num_full_philos++;
	if (philo->conf->num_full_philos == philo->conf->num_philos)
		philo->conf->finish_flag = true;
	pthread_mutex_unlock(&philo->conf->m_common);
	if (wait_action_time(philo, philo->conf->eat_ms))
		return (1);
	return (0);
}

void	*philo_main(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->conf->m_common);
	philo->last_eat_ms = get_time_ms();
	pthread_mutex_unlock(&philo->conf->m_common);
	if (philo->id % 2 == 1)
		usleep(philo->conf->eat_ms * 0.9 * 1000);
	while (1)
	{
		if (take_forks(philo->conf, philo->id)
			|| eating(philo)
			|| put_forks(philo->conf, philo->id)
			|| sleeping(philo)
			|| thinking(philo))
			break ;
	}
	put_forks(philo->conf, philo->id);//if deadを入れるか検討
	return ("finished");
}
