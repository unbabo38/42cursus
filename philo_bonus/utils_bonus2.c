/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 20:19:59 by tmura             #+#    #+#             */
/*   Updated: 2026/01/19 15:16:43 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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

void	make_sem_name(char *name, int id)
{
	int		i;
	int		j;
	char	*base;
	char	tmp[12];

	base = "/meal_lock_";
	i = 0;
	while (base[i])
	{
		name[i] = base[i];
		i++;
	}
	if (id == 0)
		name[i++] = '0';
	j = 0;
	while (id > 0)
	{
		tmp[j++] = (id % 10) + '0';
		id /= 10;
	}
	while (j > 0)
		name[i++] = tmp[--j];
	name[i] = '\0';
}
