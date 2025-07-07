/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_massages.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 20:47:16 by tmura             #+#    #+#             */
/*   Updated: 2025/07/07 18:51:54 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

void	command_not_found(char *cmd, char **args)
{
	safe_write(STDERR_FILENO, cmd, ft_strlen(cmd));
	safe_write(STDERR_FILENO, ": command not found\n", 20);
	if (args)
		free_split(args);
	exit(127);
}

void	usage(void)
{
	safe_write(STDERR_FILENO, "Bad arguments\n", 14);
	safe_write(STDERR_FILENO,
		"./pipex <file1> <cmd1> <cmd2> <file2>\n", 38);
	exit(EXIT_FAILURE);
}

void	usage_multi_pipe(void)
{
	safe_write(STDERR_FILENO, "Bad arguments\n", 14);
	safe_write(STDERR_FILENO,
		"./pipex_bonus <file1> <cmd1> <cmd2> <...> <file2>\n", 44);
	exit(EXIT_FAILURE);
}

void	usage_here_doc(void)
{
	safe_write(STDERR_FILENO, "Bad arguments\n", 14);
	safe_write(STDERR_FILENO,
		"./pipex_bonus here_doc <LIMITER> <cmd1> <...> <file2>\n", 48);
	exit(EXIT_FAILURE);
}
