#include "libft.h"
#include <string.h>
#include <stdio.h>

char *ft_strtrim(char const *s1, char const *set)
{
	size_t left;
	size_t right;

	left = 0;
	right = strlen(s1);
	if (!s1)
		return (NULL);
	if (!set)
		return (ft_strdup(s1));
	while (left < right && ft_strchr(set, s1[left]))
		left++;
	while (left < right && ft_strrchr(set, s1[right - 1]))
		right--;
	return (ft_substr(s1, left, right - left));
}
/*
int main(void)
{
	char const *s1 = "hoge";
	char const *set = "Hello,howorld";
    printf("%s", ft_strtrim(s1, set));
}
*/
