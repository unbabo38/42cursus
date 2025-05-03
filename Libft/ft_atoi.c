#include "libft.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
int	ft_atoi(const char *str)
{
	long	i;
	long	nbr;
	int		isneg;

	i = 0;
	nbr = 0;
	isneg = 0;
	while (str[i] != '\0' && (str[i] == 32 || str[i] == '\t' || str[i] == '\n'
			|| str[i] == '\r' || str[i] == '\v' || str[i] == '\f'))
		i++;
	if (str[i] != '\0' && str[i] == '-')
	{
		isneg = 1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] != '\0' && ft_isdigit(str[i]))
		nbr = (nbr * 10) + (str[i++] - '0');
	if (isneg == 1 || nbr == 2147483648)
		return (-nbr);
	if (nbr > 2147483647)
		return (-1);
	if (nbr < -2147483648)
		return (0);
	return (nbr);

}
/*
int	main(void)
{
	printf("Result 1: %d\n", ft_atoi("4294967296"));              // → 42
	printf("Result 2: %d\n", atoi("4294967296"));          // → -42
	printf("Result 1: %d\n", ft_atoi("9223372036854775808"));              // → 42
	printf("Result 2: %d\n", atoi("9223372036854775808"));
	printf("Result 1: %d\n", ft_atoi("-9223372036854775809"));              // → 42
	printf("Result 2: %d\n", atoi("-9223372036854775809"));
	printf("Result 1: %d\n", ft_atoi("18446744073709551616"));              // → 42
	printf("Result 2: %d\n", atoi("18446744073709551616"));
	printf("Result 1: %d\n", ft_atoi("18446744073709551616"));              // → 42
	printf("Result 2: %d\n", atoi("18446744073709551616"));
	return (0);
}
*/
