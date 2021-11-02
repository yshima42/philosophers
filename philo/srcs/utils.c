#include "../includes/philosophers.h"

void	*my_malloc(size_t size)
{
	void	*ptr;
	ptr = malloc(size);
	if (!ptr)
	{
		ft_putstr_fd("malloc error\n", 2);
		exit(EXIT_FAILURE);
	}
	return (ptr);
}

size_t	gettime_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec) * 1000 + (tv.tv_usec) / 1000);
}