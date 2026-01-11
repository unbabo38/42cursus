/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 13:27:23 by tmura             #+#    #+#             */
/*   Updated: 2026/01/07 13:27:23 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	break_check(t_data *data)
{
	pthread_mutex_lock(&data->meal_check);
	if (data->dead_flag || data->all_ate)
	{
		pthread_mutex_unlock(&data->meal_check);
		return (1);
	}
	return (0);
}

static int	break_check_meal_num(t_data *data, t_philo *philo)
{
	if (data->num_meals != -1 && philo->meals_eaten >= data->num_meals)
	{
		pthread_mutex_unlock(&data->meal_check);
		return (1);
	}
	return (0);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	if (philo->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		if (break_check(data))
			break ;
		if (break_check_meal_num(data, philo))
			break ;
		pthread_mutex_unlock(&data->meal_check);
		eat(philo);
		if (break_check(data))
			break ;
		pthread_mutex_unlock(&data->meal_check);
		sleep_think(philo);
		if (break_check(data))
			break ;
		pthread_mutex_unlock(&data->meal_check);
	}
	return (NULL);
}
