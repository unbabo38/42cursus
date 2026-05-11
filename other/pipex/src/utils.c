/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 10:49:47 by tmura             #+#    #+#             */
/*   Updated: 2025/07/08 16:52:17 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	safe_write(int fd, const void *buf, size_t count)
{
	if (write(fd, buf, count) == ERROR)
	{
		perror("write");
		exit(EXIT_FAILURE);
	}
}

void	safe_dup2(int fd, int std_filenum)
{
	if (dup2(fd, std_filenum) == ERROR)
	{
		perror("dup2");
		close(fd);
		exit(EXIT_FAILURE);
	}
}

void	safe_pipe(int fd[2])
{
	if (pipe(fd) == ERROR)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}

int	open_infile(char *arg)
{
	int	infile;

	infile = open(arg, O_RDONLY);
	if (infile == ERROR)
	{
		perror(arg);
		infile = open("/dev/null", O_RDONLY);
		if (infile == ERROR)
		{
			perror("open");
			exit(EXIT_FAILURE);
		}
	}
	return (infile);
}

void	safe_close(int fd)
{
	if (close(fd) == ERROR)
	{
		perror("close");
		exit(EXIT_FAILURE);
	}
}
