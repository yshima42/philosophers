#include "../includes/philosophers.h"

//あとで消す

void	print_conf(t_conf *conf)
{
	printf("[print_conf]\n");
	printf("conf->num_philos: %lu\n", conf->num_philos);
	printf("conf->die_ms: %lu\n", conf->die_ms);
	printf("conf->eat_ms: %lu\n", conf->eat_ms);
	printf("conf->sleap_ms: %lu\n", conf->sleep_ms);
	printf("conf->num_must_eat: %lu\n", conf->num_must_eat);
	printf("\n");
}

void	print_philo(t_philo **philo)
{
	size_t	i;
	
	i = -1;
	while (++i < philo[1]->conf->num_philos)
	{
		printf("[print_philo]\n");
		printf("id: %lu\n", philo[i]->id);
		printf("status: %u\n", philo[i]->status);
		printf("\n");
		print_conf(philo[i]->conf);
	}
}