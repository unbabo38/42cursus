/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 13:27:26 by tmura             #+#    #+#             */
/*   Updated: 2026/01/07 13:27:26 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	print_action(t_data *data, int id, char *str)
{
	long long	time;
	int			should_print;

	pthread_mutex_lock(&data->meal_check);
	should_print = !data->dead_flag;
	pthread_mutex_unlock(&data->meal_check);
	pthread_mutex_lock(&data->print_mutex);
	time = get_time() - data->start_time;
	if (should_print)
		printf("%lld %d %s\n", time, id, str);
	pthread_mutex_unlock(&data->print_mutex);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

int	error_msg(char *str)
{
	printf("Error: %s\n", str);
	return (1);
}
