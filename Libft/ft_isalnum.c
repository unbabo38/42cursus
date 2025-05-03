#include "libft.h"
#include <ctype.h>
#include <stdio.h>

int	ft_isalnum(int c)
{
	if (ft_isdigit(c) || ft_isalpha(c))
		return (8);
	return (0);
}
/*
int main() {
	printf("%d\n",isalnum('A'));
	printf("%d\n",isalnum('#'));
	printf("%d\n",ft_isalnum('A'));
	printf("%d\n",ft_isalnum('#'));
}
*/
