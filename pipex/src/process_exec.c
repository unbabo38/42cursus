/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 20:43:39 by tmura             #+#    #+#             */
/*   Updated: 2025/07/05 20:43:43 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"
#include "../include/pipex_bonus.h"

int	check_cmd_exist(char *arg)
{
	if (access(arg, F_OK) != 0)
	{
		perror(arg);
		exit(127);
	}
	if (access(arg, X_OK) != 0)
	{
		perror(arg);
		exit(126);
	}
	return (1);
}

char	*resolve_command(char *arg_cmd, char *cmd_env)
{
	char	**cmd_paths;
	int		i;
	char	*cmd;
	char	*joined_slash;

	cmd_paths = ft_split(cmd_env, ':');
	i = 0;
	while (cmd_paths[i])
	{
		cmd = cmd_paths[i];
		joined_slash = ft_strjoin(cmd, "/");
		cmd = ft_strjoin(joined_slash, arg_cmd);
		free(joined_slash);
		if (access(cmd, F_OK) == 0 && access(cmd, X_OK) == 0)
		{
			free_split(cmd_paths);
			return (cmd);
		}
		i++;
	}
	free_split(cmd_paths);
	return (NULL);
}

char	*get_cmd_path(char *arg_cmd, char **envp)
{
	int		i;
	char	*cmd_env;

	i = 0;
	cmd_env = NULL;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			cmd_env = envp[i] + 5;
			break ;
		}
		i++;
	}
	return (resolve_command(arg_cmd, cmd_env));
}

void	free_split(char **contents)
{
	int	i;

	i = 0;
	while (contents[i])
	{
		free(contents[i]);
		i++;
	}
	free(contents);
}

void	exec(char *arg, char **envp)
{
	char	**cmd_args;
	char	*cmd_path;

	cmd_path = NULL;
	if (arg == NULL || arg[0] == '\0')
	{
		command_not_found("\'\'");
		exit(127);
	}
	cmd_args = ft_split(arg, ' ');
	if (ft_strchr(cmd_args[0], '/'))
	{
		if (check_cmd_exist(cmd_args[0]))
			cmd_path = cmd_args[0];
	}
	else
		cmd_path = get_cmd_path(cmd_args[0], envp);
	if (!cmd_path)
	{
		command_not_found(cmd_args[0]);
		exit(127);
	}
	execve(cmd_path, cmd_args, envp);
	perror("execve failed");
	exit(EXIT_FAILURE);
}
