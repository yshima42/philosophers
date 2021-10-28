/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshimazu <yshimazu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 21:56:57 by yshimazu          #+#    #+#             */
/*   Updated: 2021/10/27 17:41:57 by yshimazu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <sys/types.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <errno.h>
# include <string.h>
# include <pthread.h>

typedef enum e_status
{
	INVALID,
	EAT,
	SLEEP,
	THINK,
	DEAD,
}	t_status;

typedef struct s_info	t_info;

typedef struct s_philo
{
	size_t			id;
	//size_t		eat_count;
	t_status		status;
	pthread_t		thread;
	t_info			*info;
}	t_philo;

typedef struct s_info
{
	size_t			num_philos;
	size_t			die_ms;
	size_t			eat_ms;
	size_t			sleep_ms;
	size_t			num_must_eat;
	t_philo			**philo;
	pthread_mutex_t	**m_forks;
}	t_info;

//ft_atoi.c
int	ft_atoi(const char *str);


#endif