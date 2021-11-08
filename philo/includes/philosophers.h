/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshimazu <yshimazu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 21:56:57 by yshimazu          #+#    #+#             */
/*   Updated: 2021/11/08 11:38:18 by yshimazu         ###   ########.fr       */
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

# define END			"\033[0m"
# define BLACK			"\033[30m"
# define RED			"\033[31m"
# define GREEN			"\033[32m"
# define YELLOW			"\033[33m"
# define BLUE			"\033[34m"
# define MAGENTA		"\033[35m"
# define CYAN			"\033[36m"

typedef enum e_status
{
	INVALID,
	EAT,
	SLEEP,
	THINK,
}	t_status;

typedef enum e_condition
{
	ALIVE,
	DEAD,
	FULL,
}	t_condition;

typedef struct s_conf	t_conf;

typedef struct s_philo
{
	size_t			id;
	size_t			eat_count;
	size_t			start_eat_ms;
	bool			has_right_fork;
	bool			has_left_fork;
/* 	t_status		status;
	pthread_mutex_t	m_status; */
	t_condition		condition;
	pthread_t		thread;
	t_conf			*conf;
}	t_philo;

typedef struct s_monitor
{
	size_t			id;
	pthread_t		thread;
	t_philo			*philo;
	t_conf			*conf;
}	t_monitor;

typedef struct s_conf
{
	size_t			num_philos;
	size_t			die_ms;
	size_t			eat_ms;
	size_t			sleep_ms;
	size_t			num_must_eat;
	bool			someone_is_dead;
	t_monitor		**monitor;
	t_philo			**philo;
	pthread_mutex_t	**m_forks;
	pthread_mutex_t	m_print;
}	t_conf;

//arg_check.c
void	args_check(int ac, char **av);

//ft_atoi.c
int	ft_atoi(const char *str);

//init_conf.c
t_conf	*init_conf(int ac, char **av);

//libft.c
void	ft_putstr_fd(char *s, int fd);

//utils.c
void	*my_malloc(size_t size);
size_t	get_time_ms(void);

//fork.c
int	take_forks(t_conf *conf, size_t id);
int put_forks(t_conf *conf, int id);
void	destroy_forks(t_conf *conf);
int	fork_mutex(bool is_lock, bool is_right, size_t id, t_conf *conf);

//main.c
void	print_action(t_conf *conf, size_t id, char *action);
int	dead_check(t_philo *philo);
bool	wait_action_time(t_philo *philo, size_t limit_ms);

#endif