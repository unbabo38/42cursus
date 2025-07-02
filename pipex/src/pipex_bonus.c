#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "../libft/libft.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int	get_next_line(char **line)
{
	char	*buffer;
	int		i;
	int		r;
	char	c;

	i = 0;
	r = 0;
	buffer = (char *)malloc(10000);
	if (!buffer)
		return (-1);
	r = read(0, &c, 1);
	if (!r)
		return (0);
	while (r && c != '\n' && c != '\0')
	{
		if (c != '\n' && c != '\0')
			buffer[i] = c;
		i++;
		r = read(0, &c, 1);
	}
	buffer[i] = '\n';
	buffer[++i] = '\0';
	*line = buffer;
	free(buffer);
	return (r);
}

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
char *resolve_command(char *arg_cmd, char *cmd_env)
{
	char **cmd_paths = ft_split(cmd_env, ':');
	int i = 0;
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
	return(resolve_command(arg_cmd, cmd_env));

}

int	check_cmd_exist(char *arg)
{
	if (access(arg, F_OK) != 0)
	{
		//fprintf(stderr, "%s: %s: %s\n", "bash", arg, strerror(errno));
		perror("bash");

		return (0);
	}
	if (access(arg, X_OK) != 0)
	{
		//fprintf(stderr, "%s: %s: %s\n", "bash", arg, strerror(errno));
		perror("bash");
		return (0);
	}
	return (1);
}

void	exec(char *arg, int argc, char **envp)
{
	char **cmd_args = ft_split(arg, ' ');
	char *cmd_path;
	if (ft_strchr(arg, '/'))
	{
		if (!check_cmd_exist)
			exit(EXIT_FAILURE);
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

void here_document(char *limiter, int argc)
{
	int fd[2];
	char *line;
	pipe(fd);
	int process = fork();

	if (process == 0)
	{
		while(1)
		{
			write(1, "> ", 2);
			if (!get_next_line(&line))
			{
				write(1, "\n", 1);
				exit(EXIT_SUCCESS);
			}
			if (ft_strncmp(limiter, line, ft_strlen(limiter)) == 0)
				exit(EXIT_SUCCESS);
			write(fd[1], line, ft_strlen(line));
		}

	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
	}
}

void do_pipe(int i, char **argv, int argc, char **envp)
{
	int     fd[2];
	pid_t   pid;
	while(i < argc - 2)
	{
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
}

void wait_children(int i, int argc)
{
	while(i < argc - 2)
	{
		wait(NULL);
		i++;
	}
}

void last_exec(char *arg, int argc, char **envp, int outfile)
{
		if (dup2(outfile, STDOUT_FILENO) == -1)
			fprintf(stderr, "%s: %s: %s\n", "bash", arg, strerror(errno));
		exec(arg, argc, envp);
		exit(EXIT_SUCCESS);
}

int process_input(char **argv, int argc)
{
	int infile;
	int i;

	i = 0;
	infile = 0;
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		i = 3;
		here_document(argv[2], argc);
	}
	else
	{
		i = 2;
		infile = open(argv[1], O_RDONLY);
		if (infile < 0)
		{
			//fprintf(stderr, "%s: %s: %s\n", "bash", argv[1], strerror(errno));
			perror("bash");

			return 0;
		}
		dup2(infile, STDIN_FILENO);
	}
	return (i);
}

int main(int argc, char **argv, char **envp)
{
	int		i;
    int     outfile;

	i = 0;
	outfile = 0;
	if (argc >= 5)
	{
		i = process_input(argv, argc);
		if (!i)
			return (0);
		outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		do_pipe(i, argv, argc, envp);
		wait_children(i, argc);
		printf("now");
		last_exec(argv[argc - 2], argc - 2, envp, outfile);
	}
	return (0);
}
