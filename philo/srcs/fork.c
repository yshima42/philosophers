/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshimazu <yshimazu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 15:14:57 by yshimazu          #+#    #+#             */
/*   Updated: 2021/11/15 16:39:49 by yshimazu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	fork_mutex(bool is_lock, bool is_right, size_t id, t_conf *conf)
{
	int				ret;
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
		ret = pthread_mutex_lock(conf->m_forks[index]);
	else
		ret = pthread_mutex_unlock(conf->m_forks[index]);
	return (ret);
}

int	take_forks(t_conf *conf, size_t id)
{
	if (fork_mutex(LOCK, RIGHT, id, conf))
		return (-1);//修正
	if (conf->finish_flag == true)
		return (true);
	print_action(conf, id, CYAN"has taken a fork"END);
	conf->philo[id - 1]->has_right_fork = true;
	if (fork_mutex(LOCK, LEFT, id, conf))
		return (-1);//修正
	if (conf->finish_flag == true)
		return (true);
	print_action(conf, id, CYAN"has taken a fork"END);//色消す
	conf->philo[id - 1]->has_left_fork = true;
	return (0);
}

int	put_forks(t_conf *conf, int id)
{
	if (fork_mutex(UNLOCK, RIGHT, id, conf))
		return (-1);
	conf->philo[id - 1]->has_right_fork = false;	
	if (fork_mutex(UNLOCK, LEFT, id, conf))
		return (-1);
	conf->philo[id - 1]->has_left_fork = false;
	return (0);
}
