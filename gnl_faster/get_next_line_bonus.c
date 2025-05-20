/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:48:35 by tmura             #+#    #+#             */
/*   Updated: 2025/05/20 18:09:17 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

int	ft_getchar(int fd, t_fd_state *state)
{
	int	r;

	if (state->bytes == 0)
	{
		r = read(fd, state->buffer, BUFFER_SIZE);
		if (r < 0)
			return (-42);
		if (r == 0)
			return (EOF);
		state->bytes = r;
		state->bufp = state->buffer;
	}
	state->bytes--;
	return (*state->bufp++);
}

int	ft_insert_char_to_line(t_fd_state *state, char c)
{
	if (!state->str.line)
	{
		state->str.saved_size = 16;
		state->str.line = malloc(sizeof(char) * state->str.saved_size);
		if (!state->str.line)
			return (-1);
	}
	if (state->str.line_length + 2 >= state->str.saved_size)
		if (expand_line_buffer(state) == -1)
			return (-1);
	state->str.line[state->str.line_length++] = c;
	return (0);
}

char	*finalize_and_return(t_fd_state *state)
{
	char	*ret;

	if (ft_insert_char_to_line(state, '\0') == -1)
		return (handle_error(state));
	ret = ft_strdup(state->str.line);
	if (!ret)
		return (handle_error(state));
	free_and_reset(state);
	return (ret);
}

int	read_loop(int fd, t_fd_state *state)
{
	int	c;

	while (1)
	{
		c = ft_getchar(fd, state);
		if (c == -42)
			return (-1);
		if (c == EOF)
			return (0);
		if (ft_insert_char_to_line(state, c) == -1)
			return (-1);
		if (c == '\n')
			break ;
	}
	return (1);
}

char	*get_next_line(int fd)
{
	static t_fd_state	fds[4086];
	t_fd_state			*state;
	int					res;

	if (fd < 0 || fd >= 4086 || BUFFER_SIZE <= 0)
		return (NULL);
	state = &fds[fd];
	res = read_loop(fd, state);
	if (res == -1)
		return (handle_error(state));
	if (state->str.line_length > 0 || state->str.line != NULL)
		return (finalize_and_return(state));
	return (handle_error(state));
}
