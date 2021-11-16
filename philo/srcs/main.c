/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshimazu <yshimazu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 14:44:08 by yshimazu          #+#    #+#             */
/*   Updated: 2021/11/16 15:33:20 by yshimazu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	philo_create(t_conf *conf)
{
	size_t	i;

	i = -1;
	while (++i < conf->num_philos)
	{
		if (pthread_create(&conf->philo[i]->thread, NULL,
				philo_main, (void *)conf->philo[i]) != 0)
		{
			printf("pthread_create error\n");
			return (EXIT_FAILURE);
		}
	}
	return (0);
}

int	monitor_create(t_conf *conf)
{
	size_t	i;

	i = -1;
	while (++i < conf->num_philos)
	{
		if (pthread_create(&conf->monitor[i]->thread, NULL,
				monitor_main, (void *)conf->monitor[i]) != 0)
		{
			printf("pthread_create error\n");
			return (EXIT_FAILURE);
		}
	}
	return (0);
}

int	philo_join(t_conf *conf)
{
	size_t	i;

	i = -1;
	while (++i < conf->num_philos)
	{
		if (pthread_join(conf->philo[i]->thread, NULL) != 0)
		{
			printf ("join error\n");
			return (EXIT_FAILURE);
		}
	}
	return (0);
}

int	monitor_join(t_conf *conf)
{
	size_t	i;

	i = -1;
	while (++i < conf->num_philos)
	{
		if (pthread_join(conf->monitor[i]->thread, NULL) != 0)
		{
			printf ("join error\n");
			return (EXIT_FAILURE);
		}
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_conf	*conf;

	if (!args_check(ac, av))
		return (EXIT_FAILURE);
	conf = init_conf(ac, av);
	if (philo_create(conf) || monitor_create(conf))
	{
		all_free_destroy(conf);
		return (EXIT_FAILURE);
	}
	if (philo_join(conf) || monitor_join(conf))
	{
		all_free_destroy(conf);
		return (EXIT_FAILURE);
	}
	all_free_destroy(conf);
	return (EXIT_SUCCESS);
}
