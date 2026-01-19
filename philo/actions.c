/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 13:27:12 by tmura             #+#    #+#             */
/*   Updated: 2026/01/19 14:33:09 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	fork_meal(t_data *data, t_philo *philo,
	pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	pthread_mutex_lock(&data->meal_check);
	if (data->dead_flag || data->all_ate)
	{
		pthread_mutex_unlock(&data->meal_check);
		pthread_mutex_unlock(first_fork);
		return ;
	}
	pthread_mutex_unlock(&data->meal_check);
	pthread_mutex_lock(second_fork);
	print_action(data, philo->id, "has taken a fork");
	pthread_mutex_lock(&data->meal_check);
	if (data->dead_flag || data->all_ate)
	{
		pthread_mutex_unlock(&data->meal_check);
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(second_fork);
		return ;
	}
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&data->meal_check);
	print_action(data, philo->id, "is eating");
}

static void	take_eat(t_data *data,
	pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	long long	start_time;

	start_time = get_time();
	while (get_time() - start_time < data->time_to_eat)
	{
		pthread_mutex_lock(&data->meal_check);
		if (data->dead_flag || data->all_ate)
		{
			pthread_mutex_unlock(&data->meal_check);
			pthread_mutex_unlock(first_fork);
			pthread_mutex_unlock(second_fork);
			return ;
		}
		pthread_mutex_unlock(&data->meal_check);
		precise_usleep(100, data);
	}
	pthread_mutex_unlock(first_fork);
	pthread_mutex_unlock(second_fork);
}

static void	take_first_fork(pthread_mutex_t *first_fork,
	t_data *data, t_philo *philo)
{
	pthread_mutex_lock(first_fork);
	print_action(data, philo->id, "has taken a fork");
	if (data->num_philos == 1)
	{
		usleep(data->time_to_die * 1000);
		pthread_mutex_unlock(first_fork);
		return ;
	}
}

void	eat(t_philo *philo)
{
	t_data				*data;
	pthread_mutex_t		*first_fork;
	pthread_mutex_t		*second_fork;

	data = philo->data;
	pthread_mutex_lock(&data->meal_check);
	if (data->dead_flag || data->all_ate)
	{
		pthread_mutex_unlock(&data->meal_check);
		return ;
	}
	pthread_mutex_unlock(&data->meal_check);
	if (philo->id % 2 == 0)
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	else
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	take_first_fork(first_fork, data, philo);
	fork_meal(data, philo, first_fork, second_fork);
	take_eat(data, first_fork, second_fork);
}

void	sleep_think(t_philo *philo)
{
	t_data		*data;
	long long	think_time;

	data = philo->data;
	print_action(data, philo->id, "is sleeping");
	precise_usleep(data->time_to_sleep, data);
	print_action(data, philo->id, "is thinking");
	if (data->num_philos % 2 != 0)
	{
		think_time = (data->time_to_die
				- (data->time_to_eat + data->time_to_sleep)) / 2;
		if (think_time > 0)
			usleep(think_time * 1000);
	}
}
