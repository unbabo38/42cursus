/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 20:47:57 by tmura             #+#    #+#             */
/*   Updated: 2025/07/06 13:32:24 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

char *get_env_value(char *name, char **envp)
{
    int len;
    int i;

    len = ft_strlen(name);
    i = 0;
    while (envp[i])
    {
        if (!ft_strncmp(envp[i], name, len) && envp[i][len] == '=')
            return (envp[i] + len + 1);
        i++;
    }
    return (NULL);
}

static char *extract_var_name(const char *line, int *i)
{
    int     start;
    char    save;
    char    *var;

    start = *i;
    while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
        (*i)++;
    save = line[*i];
    ((char *)line)[*i] = '\0';
    var = ft_strdup(line + start);
    ((char *)line)[*i] = save;
    return (var);
}

static char *append_value(char *res, char *value)
{
    char *tmp;

    tmp = ft_strjoin(res, value);
    free(res);
    return (tmp);
}

char *expand_env(char *line, char **envp)
{
    char    *result;
    char    *var;
    char    *val;
    char    tmp[2];
    int     i;

    result = ft_strdup("");
    i = 0;
    while (line[i])
    {
        if (line[i] == '\\' && line[i + 1] == '$')
        {
            tmp[0] = '$';
            tmp[1] = '\0';
            result = append_value(result, tmp);
            i += 2;
        }
        else if (line[i] == '$')
        {
            i++;
            var = extract_var_name(line, &i);
            val = get_env_value(var, envp);
            free(var);
            result = append_value(result, val ? val : "");
        }
        else
        {
            tmp[0] = line[i++];
            tmp[1] = '\0';
            result = append_value(result, tmp);
        }
    }
    return (result);
}

char	*expand_buffer(char *buffer, int *capacity, int length)
{
	int		new_capacity;
	char	*new_buffer;
	int		i;

	new_capacity = *capacity * 2;
	new_buffer = malloc(new_capacity);
	if (!new_buffer)
	{
		free(buffer);
		return (NULL);
	}
	i = 0;
	while (i < length)
	{
		new_buffer[i] = buffer[i];
		i++;
	}
	free(buffer);
	*capacity = new_capacity;
	return (new_buffer);
}

int	read_line(char *buffer, int capacity)
{
	int		size;
	char	c;
    int     i;

    i = 0;
	while ((size = read(STDIN_FILENO, &c, 1)) > 0)
	{
		if (c == '\n')
            break;
        if (i + 2 >= capacity)
		{
			buffer = expand_buffer(buffer, &capacity, i);
			if (!buffer)
				return (ERROR);
		}
		buffer[i++] = c;
	}
    buffer[i] = '\0';
    if(size < 0)
        return (ERROR);
    if (size == 0 && i == 0)
        return (0);
    return (1);
}

int	get_next_line(char **line)
{
	char	*buffer;
	int		capacity;
	int		read_size;

	capacity = INIT_CAPACITY;
	buffer = malloc(capacity);
	if (!buffer)
		return (ERROR);
	read_size = read_line(buffer, capacity);
    if (read_size == ERROR)
    {
        free(buffer);
        return (ERROR);
    }
	//buffer[i] = '\0';
	*line = buffer;
	return (read_size);
}



void	put_line(char *limiter, int fd[2], char **envp, int expand)
{
	char	*line;

	safe_write(STDOUT_FILENO, "> ", 2);
	if (get_next_line(&line) == 0)
	{
		safe_write(STDOUT_FILENO, "\n", 1);
        free(line);
		exit(EXIT_SUCCESS);
	}
	if (ft_strncmp(limiter, line, ft_strlen(limiter)) == 0)
	{
		free(line);
		exit(EXIT_SUCCESS);
	}
	if (expand)
	{
		char *expanded = expand_env(line, envp);
		safe_write(fd[1], expanded, ft_strlen(expanded));
		free(expanded);
	}
	else
		safe_write(fd[1], line, ft_strlen(line));
	safe_write(fd[1], "\n", 1);
	free(line);
}

int detect_expand(const char *limiter)
{
    if (!limiter)
        return (true);
    if (limiter[0] == '\"' || limiter[0] == '\'')
        return (false);
    return (true);  // デフォルト return を追加
}

void	here_document(char *limiter, char **envp)
{
	int	fd[2];
	int	process;

	safe_pipe(fd);
	process = fork();
	if (process == ERROR)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (process == CHILD)
	{
	    int expand;

	    expand = detect_expand(limiter);
		while (1)
			put_line(limiter, fd, envp, expand);
	}
	else
	{
		safe_close(fd[1]);
		safe_dup2(fd[0], STDIN_FILENO);
		safe_close(fd[0]);
	}
}
