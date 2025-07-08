/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 19:59:07 by tmura             #+#    #+#             */
/*   Updated: 2025/07/08 20:00:04 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include <string.h>
# include "../libft/libft.h"
# include <stdbool.h>

# define ERROR -1
# define OK 1
# define CHILD 0
# define LEAST_ARGS 5
# define HERE_DOC_SIZE 6
# define INIT_CAPACITY 1024

int		get_next_line(char **line);

// utils.c
void	free_split(char **contents);
int		check_cmd_exist(char **argv);
char	*get_cmd_path(char *arg_cmd, char **envp);
char	*resolve_command(char *arg_cmd, char *cmd_env);

// exec.c
void	exec(char *arg, char **envp);

// heredoc.c
void	here_document(char *limiter, char **envp);

// pipex.c
void	do_pipe_bonus(int i, char **argv, int argc, char **envp);
void	last_exec_bonus(char **argv, int argc, char **envp, int i);
int		process_input_bonus(char **argv, char **envp, int argc);

// optional helper (safe wrapper)
void	safe_write(int fd, const void *buf, size_t n);
void	safe_dup2(int fd, int std_filenum);
void	safe_pipe(int fd[2]);
void	safe_close(int fd);
int		open_infile(char *arg);
void	put_line(char *limiter, int fd[2], char **envp, int expand);
void	command_not_found(char *cmd, char **args);
void	usage_bonus(void);

char	*get_env_value(char *name, char **envp);
char	*extract_var_name(const char *line, int *i);
char	*append_value(char *res, char *value);
char	*expand_env(char *line, char **envp);
char	*expand_buffer(char *buffer, int *capacity, int length);
char	*next_token(const char *line, int *idx, char **envp);
int		detect_expand(const char *limiter);
void	safe_close_and_exit(int fd);
#endif
