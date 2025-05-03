#include "libft.h"
#include <string.h>
#include <stdio.h>

char *ft_substr(char const *s, unsigned int start, size_t len)
{
	char *substr_s;
	char *dest;
	const char	*src;
	if (!s)
		return (NULL);
	if ((size_t)start > ft_strlen(s))
		return (ft_strdup(""));
	substr_s = malloc(sizeof(char) * len + 1);
	if (!substr_s)
		return (NULL);
	dest = substr_s;
	src = s + start;
	while (len-- && *src)
		*dest++ = *src++;
	*dest = '\0';
	return (substr_s);
}
/*
int main(void)
{
	printf("%s", ft_substr("42tokyo", 2, 3));
}
*/
