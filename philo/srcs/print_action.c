/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_action.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yshimazu <yshimazu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 15:11:53 by yshimazu          #+#    #+#             */
/*   Updated: 2021/11/16 11:12:20 by yshimazu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	print_action(t_conf *conf, size_t id, char *action)
{
	pthread_mutex_lock(&conf->mutex_common);
	if (conf->finish_flag == false)
		printf("%ld %ld %s\n", get_time_ms(), id, action);
	pthread_mutex_unlock(&conf->mutex_common);
}
