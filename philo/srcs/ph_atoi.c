/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshimazu <yshimazu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 14:44:16 by yshimazu          #+#    #+#             */
/*   Updated: 2021/11/15 15:10:26 by yshimazu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static char	*del_spaces(char const *str)
{
	char	*no_spaces_str;
	size_t	i;

	i = 0;
	while (str[i] == '\t' || str[i] == '\n' || str[i] == '\r'
		|| str[i] == '\v' || str[i] == '\f' || str[i] == ' ')
		i++;
	no_spaces_str = &((char *)str)[i];
	return (no_spaces_str);
}

static bool	max_check(size_t result, char *ns_str, size_t i)
{
	size_t	temp;

	temp = result * 10 + (ns_str[i] - '0');
	if (temp / 10 != result)
		return (false);
	else
		return (true);
}

static bool	num_check(char *no_spaces_str, size_t i)
{
	if (no_spaces_str[i] < '0' || no_spaces_str[i] > '9')
		return (false);
	if (no_spaces_str[i] == '0' && no_spaces_str[i + 1] != '\0')
		return (false);
	return (true);
}

int	ph_atoi(const char *str)
{
	size_t	i;
	size_t	result;
	char	*no_spaces_str;

	i = 0;
	result = 0;
	no_spaces_str = del_spaces(str);
	if (!num_check(no_spaces_str, i))
		return (-1);
	while (no_spaces_str[i] >= '0' && no_spaces_str[i] <= '9')
	{
		if (!max_check(result, no_spaces_str, i))
			return (-1);
		result = result * 10 + (no_spaces_str[i] - '0');
		i++;
	}
	if (no_spaces_str[i])
		return (-1);
	return ((int)result);
}
