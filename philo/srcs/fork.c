/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshimazu <yshimazu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 15:14:57 by yshimazu          #+#    #+#             */
/*   Updated: 2021/11/16 10:53:12 by yshimazu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	fork_mutex(bool is_lock, bool is_right, size_t id, t_conf *conf)
{
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
		pthread_mutex_lock(conf->m_forks[index]);
	else
		pthread_mutex_unlock(conf->m_forks[index]);
}

int	take_forks(t_conf *conf, size_t id)
{
	fork_mutex(LOCK, RIGHT, id, conf);
	if (finish_check(conf))
		return (1);
	print_action(conf, id, CYAN"has taken a fork"END);
	conf->philo[id - 1]->has_right_fork = true;
	if (conf->num_philos == 1)
		return (1);
	fork_mutex(LOCK, LEFT, id, conf);
	if (finish_check(conf))
		return (1);
	print_action(conf, id, CYAN"has taken a fork"END);//色消す
	conf->philo[id - 1]->has_left_fork = true;
	return (0);
}

int	put_forks(t_conf *conf, int id)
{
	fork_mutex(UNLOCK, RIGHT, id, conf);
	conf->philo[id - 1]->has_right_fork = false;	
	fork_mutex(UNLOCK, LEFT, id, conf);
	conf->philo[id - 1]->has_left_fork = false;
	return (0);
}
