#include "libft.h"
#include <string.h>
#include <stdio.h>

static int ft_numlen(int n)
{
	long num = n;
	int len = (n <= 0) ? 1 : 0;
	num = (num < 0) ? -num : num;

	while (num)
	{
		num /= 10;
		len++;
	}
	return len;
}

char *ft_itoa(int n)
{
	long num = n;
	int len = ft_numlen(n);
	char *str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);

	str[len] = '\0';
	if (n == 0)
	{
		str[0] = '0';
		return str;
	}

	if (n < 0)
	{
		str[0] = '-';
		num = -num;
	}

	while (num > 0)
	{
		str[--len] = '0' + (num % 10);
		num /= 10;
	}
	return str;
}

/*
int main(void)
{
	int values[] = {0, 42, -1234, 2147483647, -2147483648};
	for (int i = 0; i < 5; i++)
	{
		char *s = ft_itoa(values[i]);
		printf("%d → %s\n", values[i], s);
		free(s);
	}
	return 0;
}
*/
