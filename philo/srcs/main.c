/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshimazu <yshimazu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 14:44:08 by yshimazu          #+#    #+#             */
/*   Updated: 2021/11/05 16:17:42 by yshimazu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

//eixtを全部消す
// philo1人の時に対処。argで弾く

int	change_condition(t_philo *philo, t_condition condition)
{
	if (pthread_mutex_lock(&philo->m_status) != 0)
		printf("%lu: mutex can not lock\n", philo->id);
	philo->condition = condition;
	philo->conf->someone_is_dead = true;
	if (pthread_mutex_unlock(&philo->m_status) != 0)
		printf("%lu: mutex can not unlock\n", philo->id);
	return (0);
}

bool	is_dead(t_conf *conf, size_t id)
{
	if (conf->someone_is_dead == true)
		return (true);
	else if (gettime_ms() - conf->philo[id - 1]->start_eat_ms >= conf->die_ms)
		return (true);
	else
		return (false);
}

int	dead_check(t_philo *philo)
{
	if (is_dead(philo->conf, philo->id))
	{
		put_forks(philo->conf, philo->id);
		change_condition(philo, DEAD);
		print_action(philo->conf, philo->id, RED"is dead"END);
		return (1);
	}
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
