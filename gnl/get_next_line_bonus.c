/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 14:06:29 by tmura             #+#    #+#             */
/*   Updated: 2025/05/16 21:25:48 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	is_invalid_or_empty(ssize_t bytes_read, char **served)
{
	if (bytes_read < 0 || !*served || (*served)[0] == '\0')
	{
		free(*served);
		*served = NULL;
		return (1);
	}
	return (0);
}

ssize_t	read_and_store(int fd, char **served, char *buffer)
{
	ssize_t		bytes_read;
	char		*tmp;

	bytes_read = 1;
	while ((!*served || !ft_strchr(*served, '\n')) && bytes_read)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read <= 0)
			break ;
		buffer[bytes_read] = '\0';
		if (!*served)
			tmp = ft_strdup(buffer);
		else
			tmp = ft_strjoin(*served, buffer);
		if (!tmp)
			return (free(*served), *served = NULL, -1);
		free(*served);
		*served = tmp;
	}
	return (bytes_read);
}

char	*extract_line(char *served)
{
	char	*line;
	int		i;

	i = 0;
	while (served[i] && served[i] != '\n')
		i++;
	if (served[i] == '\n')
		i++;
	line = ft_substr(served, 0, i);
	return (line);
}

char	*update_remain(char *served, int line_len)
{
	char	*rest;

	rest = ft_strdup(served + line_len);
	free(served);
	return (rest);
}

char	*get_next_line(int fd)
{
	static char	*served_string[4064];
	char		*buffer;
	ssize_t		bytes_read;
	char		*line;
	int			line_len;

	if (fd < 0 || fd >= 4064 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	bytes_read = read_and_store(fd, &served_string[fd], buffer);
	free(buffer);
	if (is_invalid_or_empty(bytes_read, &served_string[fd]))
		return (NULL);
	line = extract_line(served_string[fd]);
	if (!line)
		return (free(served_string[fd]), served_string[fd] = NULL, NULL);
	line_len = ft_strlen(line);
	served_string[fd] = update_remain(served_string[fd], line_len);
	return (line);
}

/*
int main(void)
{
	int fd = open("test.txt", O_RDONLY);
	int fd2 = open("test2.txt", O_RDONLY);
	char *line;

	line = get_next_line(fd2);
	printf("%s", line);
	free(line);
	line = get_next_line(fd);
	printf("%s", line);
	free(line);
	line = get_next_line(fd);
	printf("%s", line);
	free(line);
	line = get_next_line(fd);
	printf("%s", line);
	free(line);
	return 0;
}
*/
