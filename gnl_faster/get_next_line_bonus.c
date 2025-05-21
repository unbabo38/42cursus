/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:48:35 by tmura             #+#    #+#             */
/*   Updated: 2025/05/21 10:45:49 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

int	ft_getchar(int fd, t_fd_state *state)
{
	int	read_bytes;

	if (state->bytes == 0)
	{
		read_bytes = read(fd, state->buffer, BUFFER_SIZE);
		if (read_bytes < 0)
			return (-42);
		if (read_bytes == 0)
			return (EOF);
		state->bytes = read_bytes;
		state->bufp = state->buffer;
	}
	state->bytes--;
	return (*state->bufp++);
}

int	ft_insert_char_to_line(t_string *str, char c)
{
	if (!str->line)
	{
		str->saved_size = 16;
		str->line = malloc(sizeof(char) * str->saved_size);
		if (!str->line)
			return (-1);
	}
	if (str->line_length + 2 >= str->saved_size)
		if (expand_line_buffer(str) == -1)
			return (-1);
	str->line[str->line_length++] = c;
	return (0);
}

char	*finalize_and_return(t_fd_state *state, t_string *str)
{
	char	*ret;

	if (ft_insert_char_to_line(str, '\0') == -1)
		return (handle_error(state, str));
	ret = ft_strdup(str->line);
	if (!ret)
		return (handle_error(state, str));
	free_and_reset(str);
	return (ret);
}

int	read_loop(int fd, t_fd_state *state, t_string *str)
{
	int	c;

	while (1)
	{
		c = ft_getchar(fd, state);
		if (c == -42)
			return (-1);
		if (c == EOF)
			return (0);
		if (ft_insert_char_to_line(str, c) == -1)
			return (-1);
		if (c == '\n')
			break ;
	}
	return (1);
}

char	*get_next_line(int fd)
{
	static t_fd_state	fds[1024];
	t_fd_state			*state;
	t_string			str;
	int					res;

	if (fd < 0 || fd >= 1024 || BUFFER_SIZE <= 0)
		return (NULL);
	state = &fds[fd];
	str.line = NULL;
	str.line_length = 0;
	str.saved_size = 0;
	res = read_loop(fd, state, &str);
	if (res == -1)
		return (handle_error(state, &str));
	if (str.line_length > 0 || str.line != NULL)
		return (finalize_and_return(state, &str));
	return (handle_error(state, &str));
}
