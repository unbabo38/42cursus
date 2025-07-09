/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 20:22:35 by tmura             #+#    #+#             */
/*   Updated: 2025/07/08 17:33:48 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

int	detect_expand(const char *limiter)
{
	size_t	len;

	if (!limiter)
		return (OK);
	len = ft_strlen(limiter);
	if (len >= 2
		&& ((limiter[0] == '\"' && limiter[len - 1] == '\"')
			|| (limiter[0] == '\'' && limiter[len - 1] == '\'')))
	{
		return (false);
	}
	return (OK);
}

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

void	last_exec_bonus(char **argv, int argc, char **envp, int start_num)
{
	int	outfile;
	int	process;

	process = fork();
	if (process == CHILD)
	{
		if (start_num == 3)
			outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (start_num == 2)
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

int	process_input_bonus(char **argv, char **envp, int argc)
{
	int	infile;

	infile = 0;
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		if (argc < HERE_DOC_SIZE)
			usage_bonus();
		here_document(argv[2], envp);
		return (3);
	}
	else
	{
		infile = open_infile(argv[1]);
		safe_dup2(infile, STDIN_FILENO);
		safe_close(infile);
		return (2);
	}
}

int	main(int argc, char **argv, char **envp)
{
	int		start_num;

	start_num = 0;
	if (argc < LEAST_ARGS)
		usage_bonus();
	start_num = process_input_bonus(argv, envp, argc);
	do_pipe_bonus(start_num, argv, argc, envp);
	last_exec_bonus(argv, argc, envp, start_num);
	while (wait(NULL) > 0)
		;
	return (0);
}
