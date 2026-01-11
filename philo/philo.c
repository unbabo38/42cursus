/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 13:27:06 by tmura             #+#    #+#             */
/*   Updated: 2026/01/07 13:27:06 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	cleanup(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->meal_check);
	pthread_mutex_destroy(&data->print_mutex);
}

static int	initialization(t_data *data, int argc, char **argv)
{
	if (init_data(data, argc, argv) != 0)
		return (1);
	if (init_mutexes(data) != 0)
		return (1);
	if (init_philos(data) != 0)
		return (1);
	return (0);
}

static int	thread_create(t_data *data, pthread_t *monitor_thread)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
				philo_routine, &data->philos[i]) != 0)
			return (error_msg("Thresad creation failed"));
		i++;
	}
	if (pthread_create(monitor_thread, NULL, monitor, data) != 0)
		return (error_msg("Monitor thread creation failed"));
	return (0);
}

int	main(int argc, char **argv)
{
	t_data		data;
	pthread_t	monitor_thread;
	int			i;

	if (argc < 5 || argc > 6)
	{
		return (error_msg("Usage: ./philo number_of_philosophers time_to_die "
				"time_to_eat time_to_sleep "
				"[number_of_times_each_philosopher_must_eat]"));
	}
	if (initialization(&data, argc, argv) != 0)
		return (1);
	if (thread_create(&data, &monitor_thread) != 0)
		return (1);
	i = 0;
	while (i < data.num_philos)
	{
		pthread_join(data.philos[i].thread, NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);
	cleanup(&data);
	return (0);
}
