#include "philo.h"

int	check_death(t_data *data)
{
	int	i;
	long long	time;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->meal_check);
		if (data->dead_flag)
		{
			pthread_mutex_unlock(&data->meal_check);
			return (1);
		}
		time = get_time() - data->philos[i].last_meal_time;
		if (time > data->time_to_die)
		{
			pthread_mutex_unlock(&data->meal_check);
			pthread_mutex_lock(&data->print_mutex);
			pthread_mutex_lock(&data->meal_check);
			if (!data->dead_flag)
			{
				printf("%lld %d died\n", get_time() - data->start_time, data->philos[i].id);
				data->dead_flag = true;
			}
			pthread_mutex_unlock(&data->meal_check);
			pthread_mutex_unlock(&data->print_mutex);
			return (1);
		}
		pthread_mutex_unlock(&data->meal_check);
		i++;
	}
	return (0);
}

int	check_all_ate(t_data *data)
{
	int	i;

	if (data->num_meals == -1)
		return (0);
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->meal_check);
		if (data->philos[i].meals_eaten < data->num_meals)
		{
			pthread_mutex_unlock(&data->meal_check);
			return (0);
		}
		pthread_mutex_unlock(&data->meal_check);
		i++;
	}
	return (1);
}

void	*monitor(void *arg)
{
	t_data	*data;
	int		all_ate_flag;

	data = (t_data *)arg;
	while (1)
	{
		if (check_death(data))
			break ;
		all_ate_flag = check_all_ate(data);
		if (all_ate_flag)
		{
			pthread_mutex_lock(&data->meal_check);
			data->all_ate = true;
			pthread_mutex_unlock(&data->meal_check);
			break ;
		}
		usleep(500);
	}
	return (NULL);
}

