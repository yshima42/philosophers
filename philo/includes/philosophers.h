/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshimazu <yshimazu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 21:56:57 by yshimazu          #+#    #+#             */
/*   Updated: 2021/11/03 09:36:06 by yshimazu         ###   ########.fr       */
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
# include <sys/time.h>
# include <stdbool.h>

# define LOCK	1
# define UNLOCK	0
# define RIGHT	1
# define LEFT	0

typedef enum e_status
{
	INVALID,
	EAT,
	SLEEP,
	THINK,
	DEAD,
	FULL,
}	t_status;

typedef enum e_condition
{
	ALIVE,

}	t_condition;

typedef struct s_conf	t_conf;

typedef struct s_philo
{
	size_t			id;
	size_t			eat_count;
	size_t			start_eat_ms;
	t_status		status;
	pthread_mutex_t	m_status;
	t_condition		condition;
	pthread_t		thread;
	t_conf			*conf;
}	t_philo;

typedef struct s_conf
{
	size_t			num_philos;
	size_t			die_ms;
	size_t			eat_ms;
	size_t			sleep_ms;
	size_t			num_must_eat;
	pthread_t		monitor;
	t_philo			**philo;
	pthread_mutex_t	**m_forks;
}	t_conf;

//ft_atoi.c
int	ft_atoi(const char *str);

//ft_putstr_fd.c
void	ft_putstr_fd(char *s, int fd);

//ft_strlen.c
size_t	ft_strlen(const char *s);

//args_check.c
void	args_check(int ac, char **av);

//utils.c
void	*my_malloc(size_t size);
size_t	gettime_ms(void);

//init.c
t_conf	*init_conf(int ac, char **av);
void	init_philo(t_conf *conf);
void	init_forks(t_conf *conf);


#endif