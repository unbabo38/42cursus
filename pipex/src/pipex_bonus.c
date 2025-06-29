#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "../libft/libft.h"
#include <sys/types.h>
#include <sys/wait.h>

void free_split(char **contents)
{
	int i = 0;
	while (contents[i])
	{
		free(contents[i]);
		i++;
	}
	free(contents);
}

void close_all_unused_fds(int pipe_odd[2], int pipe_even[2], int i)
{
    if (i % 2 == 1) {
        // 今回使っているのは pipe_odd
        close(pipe_odd[0]);      // 自分が書き込みだけ使うので読み口は閉じ
        close(pipe_even[0]);      // 他方の pipe は読み口も書き口も閉じ
        close(pipe_even[1]);
    } else {
        // 今回使っているのは pipe_even
        close(pipe_even[0]);
        close(pipe_odd[0]);
        close(pipe_odd[1]);
    }
}

char *get_cmd_path(char *arg_cmd, char **envp)
{
	int i = 0;
	char *cmd_env;

	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			cmd_env = envp[i] + 5;

			break;
		}
		i++;
	}
	char **cmd_paths = ft_split(cmd_env, ':');
	i = 0;
	while (cmd_paths[i])
	{
		char *cmd = cmd_paths[i];
		char *joined_slash = ft_strjoin(cmd, "/");
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

void	exec(char *arg, int argc, char **envp)
{

	char **cmd_args = ft_split(arg, ' ');
	char *cmd_path;
	if (ft_strchr(arg, '/'))
	{
		if (access(arg, F_OK) != 0)
			perror("File does not exist");
		if (access(arg, X_OK) != 0)
			perror("Permission denied");
		cmd_path = arg;
	}
	else
		cmd_path = get_cmd_path(cmd_args[0], envp);

	if (!cmd_path)
	{
		fprintf(stderr, "command '%s' not found\n", cmd_args[0]);
		exit(EXIT_FAILURE);
	}
	execve(cmd_path, cmd_args, envp);
	perror("execve failed");
	exit(EXIT_FAILURE);
}




int main(int argc, char **argv, char **envp)
{
    int     fd[2];
	int     infile;
    int     outfile;
    pid_t   pid2;
	if (argc  5)
	{
		perror("invalid num of command");
		return (0);
	}
	infile = open(argv[1], O_RDONLY);
	if (infile < 0)
	{
		perror("infile");
		return (1);
    }
	outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile < 0)
	{
		perror("outfile");
		return (1);
	}
	int	i = 2;
	dup2(infile, STDIN_FILENO);

    while (i < argc - 2)
	{
		pid_t   pid;
		if (pipe(fd) < 0)
		{
			perror("pipe error");
		}
		pid = fork();
		if (pid == 0)
		{
			close(fd[0]);
			dup2(fd[1], STDOUT_FILENO);
			exec(argv[i], argc, envp);
		}
		else
		{
			close(fd[1]);
			dup2(fd[0], STDIN_FILENO);

		}
		i++;
	}
	for (int i = 0; i < argc - 2 ; i++)
	{
		wait(NULL);
	}
	dup2(outfile, STDOUT_FILENO);
	exec(argv[argc - 2], argc - 2, envp);

	if (i == argc - 2)
		exit(EXIT_SUCCESS);
	return (0);
}
