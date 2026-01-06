#include "philo_bonus.h"

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	print_action(t_data *data, int id, char *str)
{
	long long	time;

	sem_wait(data->print_sem);
	time = get_time() - data->start_time;
	if (!data->dead_flag)
		printf("%lld %d %s\n", time, id, str);
	sem_post(data->print_sem);
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

