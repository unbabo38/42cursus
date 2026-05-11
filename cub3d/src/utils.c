/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   utils.c                                           :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: t.mura <t.mura@student.42tokyo.jp>        #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/05/06 23:21:45 by t.mura           #+#    #+#              */
/*   Updated: 2026/05/11 00:17:32 by t.mura          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	free_map(t_data *data)
{
	int	i;

	if (data->map)
	{
		i = 0;
		while (i < data->map_height)
		{
			free(data->map[i]);
			i++;
		}
		free(data->map);
		data->map = NULL;
	}
}

int	is_space(char c)
{
	if (c == 32 || c == '\t' || c == '\v' || c == '\f')
		return (1);
	return (0);
}

int	is_space_and_crlf(char c)
{
	if (c == 32 || c == '\t' || c == '\v' || c == '\f' || c == '\r' || c == '\n')
		return (1);
	return (0);
}

int	ft_is_space(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (is_space_and_crlf(str[i]))
			i++;
		else
			return (0);
	}
	return (1);
}



void	free_exit(t_data *data, int status, char *error_msg)
{
	int		i;
	char	*tmp;

	if (status == PARSE_FAILED)
	{
		write(2, "Error\n", 6);
		write(2, error_msg, ft_strlen(error_msg));
		write(2, "is not proper usage!\n", 21);
	}
	if (status == EMPTY_LINE)
	{
		free(data->map_info.line);
		write(2, "Error\n", 6);
		write(2, error_msg, ft_strlen(error_msg));
	}
	if (status == FILENAME_WRONG)
	{
		write(2, "Error\n", 6);
		write(2, "filename format wrong!\n", 23);
		write(2, error_msg, ft_strlen(error_msg));
		write(2, "\n", 1);
	}
	if (status == PATH_DUP)
	{
		write(2, "Error\npath:", 11);
		write(2, error_msg, ft_strlen(error_msg));
		write(2, "\nalready set!\n", 14);
	}
	if (data->texture.no_path)
		free(data->texture.no_path);
	if (data->texture.so_path)
		free(data->texture.so_path);
	if (data->texture.we_path)
		free(data->texture.we_path);
	if (data->texture.ea_path)
		free(data->texture.ea_path);
	if (status == FAILED)
	{
		write(2, "Error\n", 6);
		write(2, error_msg, ft_strlen(error_msg));
		write(2, "\n", 1);
	}
	if (data->trimmed)
		free(data->trimmed);
	if (data->map)
		free_map(data);
	i = 0;
	while (i < 6)
	{
		if (data->tex[i].img)
			mlx_destroy_image(data->mlx, data->tex[i].img);
		i++;
	}
	if (data->map_info.line)
	{
		free(data->map_info.line);
	}
	while ((tmp = get_next_line(data->fd)))
		free(tmp);
	if (status == 0)
	{
		write(2, error_msg, ft_strlen(error_msg));
		write(2, "\n", 1);
	}
	if (data->img)
		mlx_destroy_image(data->mlx, data->img);
	if (data->win)
		mlx_destroy_window(data->mlx, data->win);
	if (data->mlx)
	{
		mlx_destroy_display(data->mlx);
		free(data->mlx);
	}
	exit(status);
	return ;
}
