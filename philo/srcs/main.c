/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshimazu <yshimazu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 14:44:08 by yshimazu          #+#    #+#             */
/*   Updated: 2021/11/04 15:14:55 by yshimazu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

//eixtを全部消す
// philo1人の時に対処。argで弾く

bool	is_dead(t_conf *conf)
{
	size_t	i;
	i = -1;
	while (++i < conf->num_philos)
	{
		if (conf->philo[i]->status == INVALID)
			return (false);
		if (gettime_ms() - conf->philo[i]->start_eat_ms >= conf->die_ms)
			return (true);
	}
	return (false);
}

int	dead_check(t_philo *philo)
{
	if (is_dead(philo->conf))
	{
		put_forks(philo->conf, philo->id);
		change_status(philo, DEAD);
		print_action(philo->conf, philo->id, RED"is dead"END);
		return (1);
	}
	else 
		return (0);
}

bool	full_check(t_conf *conf)
{
	size_t	i;
	size_t	n_full_philo;

	n_full_philo = 0;
	i = -1;
	while (++i < conf->num_philos)
	{
		if (conf->philo[i]->status == INVALID)
			return (0);
		if (conf->philo[i]->status == FULL)
		{
			n_full_philo++;
		}
	}
	//printf("%ld\n",n_full_philo);
	if (n_full_philo == conf->num_must_eat)
		return (1);
	else
		return (0);
}

int	main(int ac, char **av)
{
	t_conf	*conf;

	args_check(ac, av);
	conf = init_conf(ac, av);
	init_philo(conf);
	init_forks(conf);
	philo_create(conf);
	philo_join(conf);
	destroy_forks(conf);
	//pthread_mutex_destroy(&conf->philo[i]->m_status);
	printf("end of file\n");
}