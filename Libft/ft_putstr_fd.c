#include "libft.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>

void ft_putstr_fd(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
}
/*
int main(void)
{

	ft_putstr_fd("suzumenooyado", 1);

}
*/
