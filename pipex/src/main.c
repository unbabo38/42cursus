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
		cmd = ft_strjoin(cmd, "/");
		cmd = ft_strjoin(cmd, arg_cmd);
		if (access(cmd, X_OK) == 0)
		{

			free_split(cmd_paths);
			return (cmd);
		}
		i++;
	}
	free_split(cmd_paths);
	return (NULL);
}

void fork_odd()
{

}

void fork_even()

int main(int argc, char **argv, char **envp)
{
    int     fd[2];
    int     infile;
    int     outfile;
    pid_t   pid1;
    pid_t   pid2;
    // 1. 引数チェック（argc != 5 のときエラー出力して終了）
	if (argc != 5)
		perror("invalid num of command");

    // 2. infile を O_RDONLY で open
	infile = open(argv[1] ,O_RDONLY);
    //    - 失敗したら perror + exit
	if (infile < 0)
	{
		perror("infile");
		return (1);
    }

	// 3. outfile を O_WRONLY | O_CREAT | O_TRUNC で open
	outfile = open(argv[4], O_WRONLY);
    //    - 失敗したら perror + close(infile) + exit
	if (outfile < 0)
	{
		perror("outfile");
		return (1);
	}
    // 4. pipe(fd) する（失敗したら perror + exit）
	if (pipe(fd) < 0)
	{
		perror("pipe");
		return (1);
	}
    // 5. fork() して pid1 を作成（cmd1用の子プロセス）
    //    - pid1 == 0 のとき：cmd1 処理を実行（さっきの枠）
	pid1 = fork();
	if (pid1 == 0)
	{

		dup2(infile, STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
    	close(infile);
		printf("%s\n", argv[2]);

		char **cmd_args = ft_split(argv[2], ' ');
		char *cmd_path = get_cmd_path(cmd_args[0], envp);
		//fprintf(stderr, "%s\n", cmd_args[0]);
		execve(cmd_path, cmd_args, envp);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
    // 6. fork() して pid2 を作成（cmd2用の子プロセス）
    //    - pid2 == 0 のとき：cmd2 処理を実行（さっきの枠）

	pid2 = fork();
	if (pid2 == 0)
	{
		dup2(fd[0], STDIN_FILENO);
	   	dup2(outfile, STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		close(outfile);

		char **cmd_args = ft_split(argv[3], ' ');
		char *cmd_path = get_cmd_path(cmd_args[0], envp);
		execve(cmd_path, cmd_args, envp);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}

    // 7. 親プロセス
    //    - fd[0] と fd[1] を close
    //    - waitpid(pid1)
    //    - waitpid(pid2)
	close(fd[0]);
	close(fd[1]);
	int	status;

	waitpid(pid1, &status, 0); // cmd1 の終了を待つ
	waitpid(pid2, &status, 0); // cmd2 の終了を待つ
    // 8. return 0;
	return (0);
}
