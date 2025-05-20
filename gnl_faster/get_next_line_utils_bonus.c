/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmura <tmura@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:35:11 by tmura             #+#    #+#             */
/*   Updated: 2025/05/20 18:07:47 by tmura            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*ft_strdup(const char *s1)
{
	char	*ptr;
	size_t	len;
	size_t	i;

	len = 0;
	while (s1[len])
		len++;
	ptr = malloc(len + 1);
	if (!ptr)
		return (NULL);
	i = 0;
	while (i < len)
	{
		ptr[i] = s1[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

void	free_and_reset(t_fd_state *state)
{
	if (state->str.line)
	{
		free(state->str.line);
		state->str.line = NULL;
	}
	state->str.line_length = 0;
	state->str.saved_size = 0;
}

void	reset_fd_state(t_fd_state *state)
{
	size_t	i;

	state->bytes = 0;
	state->bufp = NULL;
	i = 0;
	while (state->buffer[i])
	{
		state->buffer[i] = 0;
		i++;
	}
}

char	*handle_error(t_fd_state *state)
{
	free_and_reset(state);
	reset_fd_state(state);
	return (NULL);
}

int	expand_line_buffer(t_fd_state *state)
{
	int		new_size;
	char	*new_line;
	int		i;

	new_size = state->str.saved_size * 2;
	new_line = malloc(sizeof(char) * new_size);
	if (!new_line)
		return (-1);
	i = 0;
	while (i < state->str.line_length)
	{
		new_line[i] = state->str.line[i];
		i++;
	}
	free(state->str.line);
	state->str.line = new_line;
	state->str.saved_size = new_size;
	return (0);
}
