#include "libft.h"
#include <string.h>
#include <stdio.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	const char	*str;
	size_t		i;
	unsigned char ch;

	str = (const char *)s;
	i = 0;
	ch = (unsigned char)c;
	while (i < n)
	{
		if (str[i] == ch)
			return ((void *)(s + i));
		i++;
	}
	return (0);
}
/*
int	main(void)
{
	const char *s = "Hello, world!";
	int	c = 'o';
	size_t	n = 10;

	void *std_result = memchr(s, c, n);
	void *ft_result = ft_memchr(s, c, n);

	printf("Input string: \"%s\"\n", s);
	printf("Character to search: '%c' (%d)\n", c, c);
	printf("Bytes to search: %zu\n\n", n);

	printf("memchr    : %s\n", (char *)std_result);
	printf("ft_memchr : %s\n", (char *)ft_result);

	if (std_result == ft_result)
		printf("✅ Test passed: results match.\n");
	else
		printf("❌ Test failed: results differ.\n");

	return 0;
}
*/
