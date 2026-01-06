#include "philo.h"

int	init_data(t_data *data, int argc, char **argv)
{
	data->num_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->num_meals = -1;
	if (argc == 6)
		data->num_meals = ft_atoi(argv[5]);
	data->dead_flag = false;
	data->all_ate = false;
	data->start_time = get_time();
	if (data->num_philos < 1 || data->num_philos > MAX_PHILOS)
		return (error_msg("Invalid number of philosophers"));
	if (data->time_to_die < 0 || data->time_to_eat < 0 || data->time_to_sleep < 0)
		return (error_msg("Invalid time values"));
	if (argc == 6 && data->num_meals < 1)
		return (error_msg("Invalid number of meals"));
	return (0);
}

int	init_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (error_msg("Mutex initialization failed"));
		i++;
	}
	if (pthread_mutex_init(&data->meal_check, NULL) != 0)
		return (error_msg("Mutex initialization failed"));
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (error_msg("Mutex initialization failed"));
	return (0);
}

int	init_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = get_time();
		data->philos[i].data = data;
		data->philos[i].left_fork = &data->forks[i];
		if (i == data->num_philos - 1)
			data->philos[i].right_fork = &data->forks[0];
		else
			data->philos[i].right_fork = &data->forks[i + 1];
		i++;
	}
	return (0);
}

