/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshimazu <yshimazu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/01 21:56:57 by yshimazu          #+#    #+#             */
/*   Updated: 2021/11/15 15:13:13 by yshimazu         ###   ########.fr       */
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

/* typedef enum e_status
{
	INVALID,
	EAT,
	SLEEP,
	THINK,
}	t_status; */

typedef enum e_condition
{
	NORMAL,
	/* DEAD, */
	FULL,
}	t_condition;

typedef struct s_conf	t_conf;

typedef struct s_philo
{
	size_t			id;
	size_t			eat_count;
	size_t			last_eat_ms;
/* 	bool			has_right_fork;
	bool			has_left_fork; */
/* 	t_status		status;
	pthread_mutex_t	m_status; */
	pthread_mutex_t	m_last_eat;
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
	pthread_mutex_t	m_someone_is_dead;
	bool			everyone_full;
	pthread_mutex_t	m_everyone_full;
	t_monitor		**monitor;
	t_philo			**philo;
	pthread_mutex_t	**m_forks;
	pthread_mutex_t	m_print;
}	t_conf;

//arg_check.c
bool	args_check(int ac, char **av);

//ph_atoi.c
int	ph_atoi(const char *str);

//init_conf.c
t_conf	*init_conf(int ac, char **av);

//utils.c
void	*my_malloc(size_t size);
size_t	get_time_ms(void);
void	ft_putstr_fd(char *s, int fd);

//fork.c
int	take_forks(t_conf *conf, size_t id);
int put_forks(t_conf *conf, int id);
int	fork_mutex(bool is_lock, bool is_right, size_t id, t_conf *conf);

//monitor_main.c
void	*monitor_main(void *arg);

//philo_main.c
void	*philo_main(void *arg);

//print_action.c
void	print_action(t_conf *conf, size_t id, char *action);

//free_destroy.c
void	destroy_all_mutex(t_conf *conf);
void	all_free(t_conf *conf);

//main.c
void	print_action(t_conf *conf, size_t id, char *action);



#endif