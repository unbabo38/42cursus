#include "libft.h"
#include <ctype.h>
#include <stdio.h>



int	ft_isdigit(int c)
{
	if ('0' <= c && c <= '9')
		return (2048);
	else
		return (0);
}
/*
int main(void) {
	printf("%d\n",ft_isdigit(2147483647));
	//printf("%d\n",isdigit('8'));
	//printf("%d", ft_isdigit('1'));
}
*/
