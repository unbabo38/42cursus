#include "get_next_line.h"
#include <stdlib.h>

size_t	ft_strlen(const char *s)
{
	size_t i = 0;
	while (s && s[i])
		i++;
	return (i);
}

char	*ft_strchr(const char *s, int c)
{
	if (!s)
		return (NULL);
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}

char	*ft_strdup(const char *s)
{
	size_t len = ft_strlen(s);
	char *dup = (char *)malloc(len + 1);
	if (!dup)
		return (NULL);
	for (size_t i = 0; i < len; i++)
		dup[i] = s[i];
	dup[len] = '\0';
	return (dup);
}

char	*ft_strjoin(char *s1, const char *s2)
{
	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));

	size_t len1 = ft_strlen(s1);
	size_t len2 = ft_strlen(s2);
	char *joined = (char *)malloc(len1 + len2 + 1);
	if (!joined)
		return (NULL);
	for (size_t i = 0; i < len1; i++)
		joined[i] = s1[i];
	for (size_t j = 0; j < len2; j++)
		joined[len1 + j] = s2[j];
	joined[len1 + len2] = '\0';
	free(s1); // 重要：メモリリークを防ぐためにs1を解放
	return (joined);
}

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	if (!s || start >= ft_strlen(s))
		return (ft_strdup(""));
	size_t max_len = ft_strlen(s + start);
	if (len > max_len)
		len = max_len;

	char *sub = (char *)malloc(len + 1);
	if (!sub)
		return (NULL);
	for (size_t i = 0; i < len; i++)
		sub[i] = s[start + i];
	sub[len] = '\0';
	return (sub);
}
