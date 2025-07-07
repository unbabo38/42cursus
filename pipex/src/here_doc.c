/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 20:47:57 by tmura             #+#    #+#             */
/*   Updated: 2025/07/06 13:32:24 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

int	read_line(char *buffer, int capacity)
{
	int		size;
	char	c;
	int		i;

	i = 0;
	while ((size = read(STDIN_FILENO, &c, 1)) > 0)
	{
		if (c == '\n')
			break ;
		if (i + 2 >= capacity)
		{
			buffer = expand_buffer(buffer, &capacity, i);
			if (!buffer)
				return (ERROR);
		}
		buffer[i++] = c;
	}
	buffer[i] = '\0';
	if (size < 0)
		return (ERROR);
	if (size == 0 && i == 0)
		return (0);
	return (1);
}

int	get_next_line(char **line)
{
	char	*buffer;
	int		capacity;
	int		read_size;

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
	*line = buffer;
	return (read_size);
}

void	put_line(char *limiter, int fd[2], char **envp, int expand)
{
	char	*line;
	char	*expanded;

	safe_write(STDOUT_FILENO, "> ", 2);
	if (get_next_line(&line) == 0)
	{
		safe_write(STDOUT_FILENO, "\n", 1);
		free(line);
		exit(EXIT_SUCCESS);
	}
	if (ft_strncmp(limiter, line, ft_strlen(limiter)) == 0)
	{
		free(line);
		exit(EXIT_SUCCESS);
	}
	if (expand)
	{
		expanded = expand_env(line, envp);
		safe_write(fd[1], expanded, ft_strlen(expanded));
		free(expanded);
	}
	else
		safe_write(fd[1], line, ft_strlen(line));
	safe_write(fd[1], "\n", 1);
	free(line);
}

int	detect_expand(const char *limiter)
{
	if (!limiter)
		return (true);
	if (limiter[0] == '\"' || limiter[0] == '\'')
		return (false);
	return (true);
}

void	here_document(char *limiter, char **envp)
{
	int	fd[2];
	int	process;
	int	expand;

	safe_pipe(fd);
	process = fork();
	if (process == ERROR)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (process == CHILD)
	{
		expand = detect_expand(limiter);
		while (1)
			put_line(limiter, fd, envp, expand);
	}
	else
	{
		safe_close(fd[1]);
		safe_dup2(fd[0], STDIN_FILENO);
		safe_close(fd[0]);
		wait(NULL);
	}
}
