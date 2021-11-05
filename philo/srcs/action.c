#include "../includes/philosophers.h"

void	print_action(t_conf *conf, size_t id, char *action)
{
	pthread_mutex_lock(&conf->m_print);
	printf("%ld %ld %s\n", gettime_ms(), id, action);//これで良いのか
	pthread_mutex_unlock(&conf->m_print);
}

bool	take_action(t_philo *philo, size_t limit_ms)
{
	size_t	start_ms;
	size_t	current_ms;
	size_t	diff;

	start_ms = gettime_ms();
	while (1)
	{
		current_ms = gettime_ms();
		diff = current_ms - start_ms;
		if (limit_ms <= diff)
			break ;
		if (dead_check(philo))
			return (true);
	}	
	return (false);
}

int	change_status(t_philo *philo, t_status status)
{
	if (pthread_mutex_lock(&philo->m_status) != 0)
		printf("%lu: mutex can not lock\n", philo->id);
	philo->status = status;
	if (pthread_mutex_unlock(&philo->m_status) != 0)
		printf("%lu: mutex can not unlock\n", philo->id);
	return (0);
}

int	eating(t_philo *philo)
{
	if (philo->conf->someone_is_dead)
		return (1);
	print_action(philo->conf, philo->id, GREEN"is eating"END);
	philo->start_eat_ms = gettime_ms();
	printf("id: %zu, philo->start_eat_ms: %zu\n", philo->id, philo->start_eat_ms);
	take_action(philo, philo->conf->eat_ms);
	philo->eat_count++;
	if (philo->eat_count == philo->conf->num_must_eat)
		philo->condition = FULL;
	if (philo->condition == DEAD)
		return (1);
	else
		change_status(philo, SLEEP);
	return (0);
}

int	sleeping(t_philo *philo)
{
	if (philo->conf->someone_is_dead)
		return (1);
	print_action(philo->conf, philo->id, BLUE"is sleeping"END);
	take_action(philo, philo->conf->sleep_ms);
	if (philo->condition == DEAD)
		return (1);
	else
		change_status(philo, THINK);
	return (0);
}

int	thinking(t_philo *philo)
{
	if (philo->conf->someone_is_dead)
		return (1);
	print_action(philo->conf, philo->id, YELLOW"is thinking"END);
	if (philo->condition == DEAD)
		return (1);
	else
		change_status(philo, EAT);
	usleep(100);//要検討
	return (0);
}