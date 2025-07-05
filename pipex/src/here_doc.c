/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 20:47:57 by tmura             #+#    #+#             */
/*   Updated: 2025/07/05 20:58:36 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

char	*expand_buffer(char *buffer, int *capacity, int length)
{
	int		new_capacity;
	char	*new_buffer;
	int		i;

	new_capacity = *capacity * 2;
	new_buffer = malloc(new_capacity);
	if (!new_buffer)
	{
		free(buffer);
		return (NULL);
	}
	i = 0;
	while (i < length)
	{
		new_buffer[i] = buffer[i];
		i++;
	}
	free(buffer);
	*capacity = new_capacity;
	return (new_buffer);
}

int	read_line(char *buffer, int capacity)
{
	int		size;
	char	c;
    int     i;

    i = 0;
	while ((size = read(STDIN_FILENO, &c, 1)) > 0)
	{
		if (c == '\n')
            break;
        if (i + 2 >= capacity)
		{
			buffer = expand_buffer(buffer, &capacity, i);
			if (!buffer)
				return (ERROR);
		}
		buffer[i++] = c;
	}
    buffer[i] = '\0';
    if(size < 0)
        return (ERROR);
    if (size == 0 && i == 0)
        return (0);
    return (1);
}

int	get_next_line(char **line)
{
	char	*buffer;
	int		capacity;
	int		i;
	int		read_size;

	i = 0;
	capacity = INIT_CAPACITY;
	buffer = malloc(capacity);
	if (!buffer)
		return (ERROR);
	read_size = read_line(buffer, capacity);
    if (read_size == ERROR)
    {
        free(buffer);
        return (ERROR);
    }
	//buffer[i] = '\0';
	*line = buffer;
	return (read_size);
}

void	put_line(char *limiter, int fd[2])
{
	char	*line;

	safe_write(STDOUT_FILENO, "> ", 2);
	if (get_next_line(&line) == 0)
	{
		safe_write(STDOUT_FILENO, "\n", 1);
		exit(EXIT_SUCCESS);
	}

	if (ft_strncmp(limiter, line, ft_strlen(limiter)) == 0)
	{
		free(line);
		exit(EXIT_SUCCESS);
	}
	safe_write(fd[1], line, ft_strlen(line));
    safe_write(fd[1], "\n", 1);
	free(line);
}

void	here_document(char *limiter)
{
	int	fd[2];
	int	process;

	safe_pipe(fd);
	process = fork();
	if (process == ERROR)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (process == CHILD)
	{
		while (1)
			put_line(limiter, fd);
	}
	else
	{
		safe_close(fd[1]);
		safe_dup2(fd[0], STDIN_FILENO);
		safe_close(fd[0]);
	}
}
