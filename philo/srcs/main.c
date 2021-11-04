/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshimazu <yshimazu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 14:44:08 by yshimazu          #+#    #+#             */
/*   Updated: 2021/11/04 15:13:51 by yshimazu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

//eixtを全部消す
// philo1人の時に対処。argで弾く

/* int	change_condition(t_philo *philo, t_status status)
{
	if (pthread_mutex_lock(&philo->m_status) != 0)
		printf("%lu: mutex can not lock\n", philo->id);
	if (status == DEAD)
	{
		philo->status = DEAD;
		print_action(philo->conf, philo->id, "is dead");
	}
	if (status == FULL)
	{
		philo->status = FULL;
		print_action(philo->conf, philo->id, "is full");
	}
	if (pthread_mutex_unlock(&philo->m_status) != 0)
		printf("%lu: mutex can not unlock\n", philo->id);
	return (0);
} */

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

/* int	change_condition_no_print(t_philo *philo, t_status status)
{
	if (pthread_mutex_lock(&philo->m_status) != 0)
		printf("%lu: mutex can not lock\n", philo->id);
	if (status == DEAD)
	{
		philo->status = DEAD;
		//print_action(philo->conf, philo->id, "is dead");
	}
	if (status == FULL)
	{
		philo->status = FULL;
		//print_action(philo->conf, philo->id, "is full");
	}
	if (pthread_mutex_unlock(&philo->m_status) != 0)
		printf("%lu: mutex can not unlock\n", philo->id);
	return (0);
} */

/* void	finish_threads(t_conf *conf)
{
	size_t	i;

	i = -1;
	while (++i < conf->num_philos)
	{
		if (conf->philo[i]->status != DEAD)
		{
			change_condition_no_print(conf->philo[i], DEAD);
		}
	}
} */

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

/* void	*monitor_main(void *arg)
{
	t_conf	*conf;

	conf = (t_conf *)arg;
	while (1)
	{
		if (is_dead(conf) || full_check(conf))
			break;
	}
	finish_threads(conf);
	return ("finish monitor");
} */

/* int	run_monitor(t_conf *conf)
{
	if (pthread_create(&conf->monitor, NULL, monitor_main, (void *)conf) != 0)
	{
		printf("pthread_create error\n");
		return (EXIT_FAILURE);
	}
	if (pthread_join(conf->monitor, NULL) != 0)
	{
		printf("pthread_join error\n");
		return (EXIT_FAILURE);
	}
	return (0);
}
 */
int	main(int ac, char **av)
{
	t_conf	*conf;

	args_check(ac, av);
	conf = init_conf(ac, av);
	init_philo(conf);
	init_forks(conf);
	philo_create(conf);
	//run_monitor(conf);
	philo_join(conf);
	destroy_forks(conf);
	//pthread_mutex_destroy(&conf->philo[i]->m_status);
	printf("end of file\n");
}