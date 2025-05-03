#include "libft.h"
#include <string.h>
#include <stdio.h>

static void make_words(char const *s, char c, int count, char **words)
{
	int i;
	int start;
	int end;

	i = 0;
	start = 0;
	end = 0;
	while (i < count)
	{
		while (s[start] && s[start] == c)
			start++;
		end = start;
		while (s[end] && s[end] != c)
			end++;
		if (end - start > 0)
		{
			words[i] = ft_substr(s, start, end - start);
			i++;
		}
		start = end;
	}
	words[i] = NULL;
}

char **ft_split(char const *s, char c)
{
	int count;
	char const *current = s;

	count = 0;
	int in_word = 0;

	while (*current)
	{
		if (*current != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*current == c)
		{
			in_word = 0;
		}
		current++;
	}
	printf("%d\n", count);
	char **words = malloc(sizeof(char *) * count + 1);
	if (!words)
		return (NULL);
	make_words(s, c, count, words);

	return (words);
}
/*
int main(void)
{
	const char *s = "   Hello   World  this is   C ";
	char **words = ft_split(s, ' ');
	int i = 0;
	while (words[i])
	{
		printf("word[%d]: '%s'\n", i, words[i]);
		free(words[i]);
		i++;
	}
	free(words);
	return 0;
}
*/
