#include "libft.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>

void ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
/*
int main(void)
{
	ft_putchar_fd('a', 2);
}
*/
