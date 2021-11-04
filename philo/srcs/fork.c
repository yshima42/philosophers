/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshimazu <yshimazu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 10:19:15 by yshimazu          #+#    #+#             */
/*   Updated: 2021/11/04 14:58:43 by yshimazu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	destroy_forks(t_conf *conf)
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
	if (ret != 0)
		printf("%ld: lock, unlock error", id);
	return (ret);
}

int	take_forks(t_conf *conf, size_t id)
{
	int	ret;

	ret = fork_mutex(LOCK, RIGHT, id, conf);
	if (is_dead(conf))
	{
		put_forks(conf, conf->philo[id - 1]->id);
		change_status(conf->philo[id - 1], DEAD);
		return (1);
	}
	if (conf->philo[id - 1]->status == DEAD)
		return (1);
	else
		print_action(conf, id, CYAN"has taken a fork"END);
	ret = fork_mutex(LOCK, LEFT, id, conf);
	if (conf->philo[id - 1]->status == DEAD)
		return (1);
	else
		print_action(conf, id, CYAN"has taken a fork"END);
	return (0);
}

int put_forks(t_conf *conf, int id)
{
	int	ret;

	ret = fork_mutex(UNLOCK, RIGHT, id, conf);//ここの順番検討
	if (conf->philo[id - 1]->status == DEAD)
		return (1);
	//print_action(conf, id, "has put a RIGHT fork");//消す
	ret = fork_mutex(UNLOCK, LEFT, id, conf);
	if (conf->philo[id - 1]->status == DEAD)
		return (1);
	//print_action(conf, id, "has put a LEFT fork");//消す
	return (0);
}