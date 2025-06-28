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


int main(int argc, char **argv, char **envp)
{
    int     pipe_odd[2];
	int     pipe_even[2];
    int     infile;
    int     outfile;

    pid_t   pid2;
    // 1. 引数チェック（argc != 5 のときエラー出力して終了）
	if (argc < 5)
	{
		perror("invalid num of command");
		//exit(EXIT_FAILURE);
		return (0);
	}
    // 2. infile を O_RDONLY で open
	infile = open(argv[1], O_RDONLY);
    //    - 失敗したら perror + exit
	if (infile < 0)
	{
		perror("infile");
		return (1);
    }

	// 3. outfile を O_WRONLY | O_CREAT | O_TRUNC で open
	outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    //    - 失敗したら perror + close(infile) + exit
	if (outfile < 0)
	{
		perror("outfile");
		return (1);
	}
	int	i = 2;
    while (i < argc - 1)
	{
		pid_t   pid;
		if (i % 2 == 1)
		{
			close(pipe_odd[0]);
    		close(pipe_odd[1]);
			if (pipe(pipe_odd) < 0)
			{
				perror("pipe1");
				return (1);
			}
		}
		else
		{
		    close(pipe_even[0]);
    		close(pipe_even[1]);
			if (pipe(pipe_even) < 0)
			{
				perror("pipe2");
				return (1);
			}
		}
    // 5. fork() して pid1 を作成（cmd1用の子プロセス）
    //    - pid1 == 0 のとき：cmd1 処理を実行（さっきの枠）
		pid = fork();
		if (pid == 0)
		{
			if (i == 2)
				dup2(infile, STDIN_FILENO);
			else if (i % 2 == 0)
				dup2(pipe_odd[0], STDIN_FILENO);
			else
				dup2(pipe_even[0], STDIN_FILENO);

			if (i == argc - 2)
				dup2(outfile, STDOUT_FILENO);
			else if (i % 2 == 0)
				dup2(pipe_even[1], STDOUT_FILENO);
			else
				dup2(pipe_odd[1], STDOUT_FILENO);

			close_all_unused_fds(pipe_odd, pipe_even, i);
			if (i == argc - 1)
				exit(EXIT_SUCCESS);
			char **cmd_args = ft_split(argv[i], ' ');
			char *cmd_path;
			if (ft_strchr(argv[i], '/'))
			{
				if (access(argv[i], F_OK) != 0)
					perror(argv[i]);
				if (access(argv[i], X_OK) != 0)
					perror("Permission denied");

				cmd_path = argv[i];
			}
			else
			{
				cmd_path = get_cmd_path(cmd_args[0], envp);
			}

			if (!cmd_path)
			{
				i++;
				fprintf(stderr, "command '%s' not found\n", cmd_args[0]);
				exit(EXIT_FAILURE);
			}
			execve(cmd_path, cmd_args, envp);
			perror("execve failed");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	close(pipe_odd[0]);
	close(pipe_odd[1]);
	close(pipe_even[0]);
	close(pipe_even[1]);

	for (int j = 0; j < argc - 3; j++)
		wait(NULL);
	return (0);
}
