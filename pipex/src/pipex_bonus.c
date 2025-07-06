/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 20:22:35 by tmura             #+#    #+#             */
/*   Updated: 2025/07/06 13:28:04 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"
#include <stdbool.h>
void	do_pipe_bonus(int i, char **argv, int argc, char **envp)
{
	int		fd[2];
	pid_t	process;

	while (i < argc - 2)
	{
		safe_pipe(fd);
		process = fork();
		if (process == CHILD)
		{
			safe_close(fd[0]);
			safe_dup2(fd[1], STDOUT_FILENO);
			safe_close(fd[1]);
			exec(argv[i], envp);
		}
		else
		{
			safe_close(fd[1]);
			safe_dup2(fd[0], STDIN_FILENO);
			safe_close(fd[0]);
		}
		i++;
	}
}

void	wait_children(int i, int argc)
{
	while (i < argc - 2)
	{
		wait(NULL);
		i++;
	}
}

void	last_exec(char *arg, char **envp, int outfile)
{
	safe_dup2(outfile, STDOUT_FILENO);
	safe_close(outfile);
	exec(arg, envp);
	exit(EXIT_SUCCESS);
}

int	process_input(char **argv, char **envp)
{
	int	infile;
	int	i;

	i = 0;
	infile = 0;
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		i = 3;
		here_document(argv[2], envp);
	}
	else
	{
		i = 2;
		infile = open_infile(argv[1]);
		safe_dup2(infile, STDIN_FILENO);
		safe_close(infile);
	}
	return (i);
}

int	main(int argc, char **argv, char **envp)
{
	int		i;
	int		outfile;

	i = 0;
	outfile = 0;
	if (argc < LEAST_ARGS_BONUS)
		usage_bonus();
	i = process_input(argv, envp);
	if (!i)
		return (0);
	do_pipe_bonus(i, argv, argc, envp);
	wait_children(i, argc);
	if (i == 3)
		outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile == ERROR)
	{
		perror(argv[argc - 1]);
		exit(EXIT_FAILURE);
	}
	last_exec(argv[argc - 2], envp, outfile);
	return (0);
}
