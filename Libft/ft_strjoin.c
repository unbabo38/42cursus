#include "libft.h"
#include <string.h>
#include <stdio.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	if (!s1 || !s2)
		return (NULL);
	int length_of_s1_and_s2;
	char *s;
	char *joined_s;
	length_of_s1_and_s2 = ft_strlen(s1) + ft_strlen(s2);
	s = malloc(sizeof(char) * (length_of_s1_and_s2 + 1));
	joined_s = s;
	while(*s1)
		*s++ = *s1++;
	while(*s2)
		*s++ = *s2++;
	*s = '\0';
	return (joined_s);
}
/*
int main(void)
{
    const char *s1 = "Hello";
	const char *s2 = "World";

    printf("%s", ft_strjoin(s1, s2));

    return 0;
}
*/
