/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 14:06:29 by tmura             #+#    #+#             */
/*   Updated: 2025/05/17 02:25:15 by tmura            ###   ########.fr       */
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
	while (bytes_read > 0 && (!*served || !ft_strchr(*served, '\n')))
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
		{
			free(*served);
			*served = NULL;
			return (-1);
		}
		free(*served);
		*served = tmp;
	}
	return (bytes_read);
}

char	*extract_line(char *served)
{
	char	*line;
	int		i;
	//printf("befor_line %s ", served);
	if (!served)
		return (NULL);
	i = 0;
	while (served[i] && served[i] != '\n')
		i++;
	if (served[i] == '\n')
		i++;
	line = ft_substr(served, 0, i);
	if (!line)
		return (NULL);
	//printf("made_line %s ", line);

	return (line);
}

char	*update_remain(char *served, int line_len)
{
	char	*rest;
	//printf("befor_line %s ", served);

	rest = ft_substr(served, line_len, ft_strlen(served) - line_len);
	if (!rest)
	{
		free(served);
		served = NULL;
		return (NULL);
	}
	free(served);
	served = NULL;
	//printf("made_line_update %s ", rest);

	return (rest);
}

char	*get_next_line(int fd)
{
	static char	*left_over[4064];
	char		*buffer;
	ssize_t		bytes_read;
	char		*line;
	int			line_len;

	if (fd < 0 || fd >= 4064 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer) {
		free(left_over[fd]);
		left_over[fd] = NULL;
		return NULL;
	}
	bytes_read = read_and_store(fd, &left_over[fd], buffer);
	free(buffer);
	if (bytes_read == -1)
		return (NULL);
	if (is_invalid_or_empty(bytes_read, &left_over[fd]))
		return (NULL);
	line = extract_line(left_over[fd]);
	if (!line)
	{
		free(left_over[fd]);
		left_over[fd] = NULL;
		return (NULL);
	}
	line_len = ft_strlen(line);
	left_over[fd] = update_remain(left_over[fd], line_len);
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
