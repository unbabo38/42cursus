#include "../include/pipex.h"

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

void	exec(char *arg, char **envp)
{
	char	**cmd_args;
	char	*cmd_path;
	if (arg == NULL || arg[0] == '\0')
	{
		command_not_found("\'\'");
		exit(127);
	}
	cmd_args = ft_split(arg, ' ');
	if (ft_strchr(arg, '/'))
	{
		if (check_cmd_exist(arg))
			cmd_path = arg;
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
	exec(arg, envp);
	exit(EXIT_SUCCESS);
}
void	do_pipe(int i, char **argv, int argc, char **envp)
{
	int		fd[2];
	pid_t	process;
	(void)i;
	(void)argc;
	if (pipe(fd) == ERROR)
	{
		perror("pipe");
	}
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

int main(int argc, char **argv, char **envp)
{
    int     infile;
    int     outfile;
	int		i;

	i = 0;
	if (argc != 5)
	{
		usage();
		exit(EXIT_FAILURE);
	}
    infile = open_infile(argv[1]);
	safe_dup2(infile, STDIN_FILENO);
	safe_close(infile);

	outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile == ERROR)
	{
		perror(argv[argc - 1]);
		exit(EXIT_FAILURE);
	}
	do_pipe(i, argv, argc, envp);
	wait_children(i, argc);
	last_exec(argv[argc - 2], envp, outfile);
}
