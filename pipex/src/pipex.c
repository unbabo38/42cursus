/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 20:20:42 by tmura             #+#    #+#             */
/*   Updated: 2025/07/07 18:10:33 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	last_exec(char **argv, int argc, char **envp)
{
	int	outfile;
	int	process;

	process = fork();
	if (process == CHILD)
	{
		outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (outfile == ERROR)
		{
			perror(argv[argc - 1]);
			exit(EXIT_FAILURE);
		}
		safe_dup2(outfile, STDOUT_FILENO);
		safe_close(outfile);
		exec(argv[argc - 2], envp);
		exit(EXIT_SUCCESS);
	}
}

void	do_pipe(char **argv, char **envp)
{
	int		fd[2];
	pid_t	process;

	safe_pipe(fd);
	process = fork();
	if (process == CHILD)
	{
		safe_close(fd[0]);
		safe_dup2(fd[1], STDOUT_FILENO);
		safe_close(fd[1]);
		exec(argv[2], envp);
	}
	else
	{
		safe_close(fd[1]);
		safe_dup2(fd[0], STDIN_FILENO);
		safe_close(fd[0]);
	}
}

int	main(int argc, char **argv, char **envp)
{
	int	infile;

	if (argc != LEAST_ARGS)
	{
		usage();
		exit(EXIT_FAILURE);
	}
	infile = open_infile(argv[1]);
	safe_dup2(infile, STDIN_FILENO);
	safe_close(infile);
	do_pipe(argv, envp);
	last_exec(argv, argc, envp);
	while (wait(NULL) > 0)
		;
	return (0);
}
