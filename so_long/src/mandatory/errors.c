#include "../../include/so_long.h"

void	error_exit(const char *error_massages)
{
	write(STDERR_FILENO, error_massages, ft_strlen(error_massages));
	exit(1);
}
