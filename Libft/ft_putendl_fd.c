#include "libft.h"
#include <string.h>
#include <stdio.h>

void ft_putendl_fd(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
	write(fd, "\n", 1);
}
/*
int main(void)
{

	ft_putendl_fd("suzumenooyado", 1);

}
*/
