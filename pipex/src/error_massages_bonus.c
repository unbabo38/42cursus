/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_massages.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 20:47:16 by tmura             #+#    #+#             */
/*   Updated: 2025/07/08 16:40:09 by tmura            ###   ########.fr       */
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

void	usage_bonus(void)
{
	safe_write(STDERR_FILENO, "Bad arguments\n", 14);
	safe_write(STDERR_FILENO,
		"./pipex here_doc <LIMITER> <cmd1> <...> <file2>\n", 48);
	safe_write(STDERR_FILENO,
		"./pipex <file1> <cmd1> <cmd2> <...> <file2>\n", 44);
	exit(EXIT_FAILURE);
}

void	safe_close_and_exit(int fd)
{
	safe_close(fd);
	exit(EXIT_SUCCESS);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	
	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}