#include "libft.h"
#include <string.h>
#include <stdio.h>

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		i++;
	}
	return (i);
}
/*
int main() {
	printf("%zu\n",strlen("ABC"));
	printf("%zu\n",strlen(""));
	printf("%zu\n",ft_strlen("ABC"));
	printf("%zu\n",ft_strlen(""));
	return (0);
}
*/
