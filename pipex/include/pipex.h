#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include <string.h>
# include "../libft/libft.h"
// get_next_line.c

# define ERROR -1
# define OK 1
# define CHILD 0
# define LEAST_ARGS_BONUS 5

int		get_next_line(char **line);

// utils.c
void	free_split(char **contents);
int		check_cmd_exist(char *arg);
char	*get_cmd_path(char *arg_cmd, char **envp);
char	*resolve_command(char *arg_cmd, char *cmd_env);

// exec.c
void	exec(char *arg, char **envp);

// heredoc.c
void	here_document(char *limiter);

// pipex.c
void	do_pipe(char **argv, char **envp);
void	wait_children(int i, int argc);
void	last_exec(char *arg, char **envp, int outfile);
int		process_input(char **argv);

// optional helper (safe wrapper)
void	safe_write(int fd, const void *buf, size_t n);
void	safe_dup2(int fd, int std_filenum);
void	safe_pipe(int fd[2]);
void	safe_close(int fd);
int		open_infile(char *arg);
void	put_line(char *limiter, int fd[2]);
void	command_not_found(char *cmd);
void	usage();

#endif
