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

int	main(int argc, char **argv)
{
	t_data		data;
	pthread_t	monitor_thread;
	int			i;

	if (argc < 5 || argc > 6)
		return (error_msg("Usage: ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]"));
	if (init_data(&data, argc, argv) != 0)
		return (1);
	if (init_mutexes(&data) != 0)
		return (1);
	if (init_philos(&data) != 0)
		return (1);
	i = 0;
	while (i < data.num_philos)
	{
		if (pthread_create(&data.philos[i].thread, NULL, philo_routine, &data.philos[i]) != 0)
			return (error_msg("Thread creation failed"));
		i++;
	}
	if (pthread_create(&monitor_thread, NULL, monitor, &data) != 0)
		return (error_msg("Monitor thread creation failed"));
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
