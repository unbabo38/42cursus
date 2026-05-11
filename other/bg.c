/*
 * EXERCISE: BROKEN_GNL (Get Next Line)
 *
 * DESCRIPTION:
 * Implement get_next_line that reads line by line from a file descriptor.
 * This version may contain special cases or intentional "bugs".
*/
#include <stddef.h>
size_t ft_strlen(const char *s)
{
  size_t res = 0;
  while (*s)
  {
    s++;
    res++;
  }
  return res;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char *ft_strdup(const char *str)
{
	char 	*line = malloc(ft_strlen(str) + 1);
	int		i = 0;
	if (line == NULL)
		return (NULL);
	while(str[i])
	{
		line[i] = str[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

char *get_next_line(int fd)
{
	static char buffer[1334];
	char line[70000];
	static int b_pos = 0;
	static int b_read = 0;
	int i = 0;

	while(1)
	{
		if (b_pos >= b_read)
		{
			b_read = read(fd, buffer, 1334);
			b_pos = 0;
			if (b_read <= 0)
				break;
		}
		line[i++] = buffer[b_pos++];
		if (buffer[b_pos - 1] == '\n')
			break;
	}
	line[i] == '\0';
	if (i == 0)
		return (NULL);
	return (ft_strdup(line));
}
