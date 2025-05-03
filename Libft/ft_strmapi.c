#include "libft.h"
#include <string.h>
#include <stdio.h>

char *ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int i;
	char *processed_s;

	if (!s || !f)
		return (NULL);
	i = 0;
	processed_s = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!processed_s)
		return (NULL);
	while (s[i])
	{
		processed_s[i] = f(i, s[i]);
		i++;
	}
	processed_s[i] = '\0';
	return (processed_s);
}
char to_upper(unsigned int i, char c)
{
	(void)i;
	if ('a' <= c && c <= 'z')
		return c - 32;
	return c;
}
/*
int main(void)
{

	char *result = ft_strmapi("suzumenooyado", to_upper);
	if (result)
	{
		printf("%s\n", result); // 出力: SUZUMENOOYADO
		free(result);
	}
}
*/
