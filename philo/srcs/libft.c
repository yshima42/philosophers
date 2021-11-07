#include "../includes/philosophers.h"

size_t	ft_strlen(const char *s)
{
	size_t	strlen;

	strlen = 0;
	while (*s != 0)
	{
		s++;
		strlen++;
	}
	return (strlen);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ft_strlen(s));
}
