#include "libft.h"
#include <string.h>
#include <stdio.h>

char	*ft_strchr(const char *s, int c)
{
	int i;

	i = 0;
	unsigned char ch = (unsigned char)c;
	while (s[i])
	{
		if (s[i] == ch)
			return ((char *)s + i);
		i++;
	}
	if (s[i] == ch)
		return ((char *)s + i);
	return (0);
}
/*
int main(void)
{
    const char *s = "Hello World";
	//ASSERT_EQ_STR(ft_strrchr(s, 'l' + 256), strrchr(s, 'l' + 256));

    printf("Original strchr: %s\n", strchr(s, 'l' + 256));
    printf("Your ft_strchr : %s\n", ft_strchr(s, 'l' + 256));

    printf("Original strchr: %s\n", strchr(s, '\0'));
    printf("Your ft_strchr : %s\n", ft_strchr(s, '\0'));

    printf("Original strchr: %s\n", strchr(s, 'o'));
    printf("Your ft_strchr : %s\n", ft_strchr(s, 'o'));

    return 0;
}
*/
