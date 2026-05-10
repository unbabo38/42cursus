/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   map.c                                             :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: t.mura <t.mura@student.42tokyo.jp>        #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/05/09 19:41:40 by t.mura           #+#    #+#              */
/*   Updated: 2026/05/10 23:18:24 by t.mura          ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	free_stab(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

char	**copy_map(t_data *data)
{
	int	i;

	data->copy_map = malloc(sizeof(char *) * (data->map_height + 1));
	if (!data->copy_map)
		free_exit(data, 6, "malloc failed!");
	i = 0;
	while (i < data->map_height)
	{
		data->copy_map[i] = ft_strdup(data->map[i]);
		if (!data->copy_map[i])
		{
			free_stab(data->copy_map);
			free_exit(data, 6, "ft_strdup failed!");
		}
		i++;
	}
	data->copy_map[i] = NULL;
	return (data->copy_map);
}

void	check_wall(t_data *data, char **map, int h, int w)
{
	if (h < 0 || h >= data->map_height || w < 0 || w >= data->map_width)
		free_exit(data, FAILED, "wall is not closed!");
	if (!map[h])
		free_exit(data, FAILED, "map does not exist!");
	if (is_space(map[h][w]))
		free_exit(data, FAILED, "space in the map!");
	if (map[h][w] == '1' || map[h][w] == 'v')
		return ;
	map[h][w] = 'v';
	check_wall(data, map, h + 1, w);
	check_wall(data, map, h - 1, w);
	check_wall(data, map, h, w + 1);
	check_wall(data, map, h, w - 1);
}

void	set_player_position(t_data *data)
{
	data->posX = data->pointer.w + 0.5;
	data->posY = data->pointer.h + 0.5;
	default_dir(data->map[data->pointer.h][data->pointer.w], data);
	data->map[data->pointer.h][data->pointer.w] = '0';
	data->pointer.cnt++;
}

void	set_sprite_position(t_data *data)
{
	data->sprite.x = data->pointer.w + 0.5;
	data->sprite.y = data->pointer.h + 0.5;
	data->map[data->pointer.h][data->pointer.w] = '0';
	data->sprite.exist = 1;
}

int	check_characters(t_data *data)
{
	char	position;

	while (data->pointer.h < data->map_height)
	{
		data->pointer.w = 0;
		while (data->pointer.w < data->map_width)
		{
			position = data->map[data->pointer.h][data->pointer.w];
			if (position == 'N' || position == 'S'
				|| position == 'W' || position == 'E')
				set_player_position(data);
			if (position == 'K')
				set_sprite_position(data);
			data->pointer.w++;
		}
		data->pointer.h++;
	}
	if (data->pointer.cnt != 1)
		return (1);
	else
		return (0);
}

int	check_map(t_data *data)
{
	char	**cmap;

	if (check_characters(data) != 0)
	{
		free_exit(data, 1, "Error\ncheck_characters failed\n");
		return (1);
	}
	cmap = copy_map(data);
	check_wall(data, cmap, (int) data->posY, (int) data->posX);
	free_stab(cmap);
	return (0);
}

void	make_map(int fd, t_data *data)
{
	data->map = malloc(sizeof(char *) * (data->map_height + 1));
	if (!data->map)
	{
		return ;
	}
	while ((data->map_info.line = get_next_line(fd)))
	{
		if (data->i < data->file_rows - data->map_height)
		{
			data->i++;
			free(data->map_info.line);
			continue ;
		}
		if (data->i < data->file_rows)
		{
			data->map[data->idx] = malloc(sizeof(char) * (data->map_width + 1));
			if (!data->map[data->idx])
				free_exit(data, FAILED, "malloc failed!");
			ft_strlcpy(data->map[data->idx],
				data->map_info.line, data->map_width + 1);
			data->idx++;
		}
		free(data->map_info.line);
	}
	data->map[data->idx] = NULL;
}

int	all_dir_texture_is_valid(t_data *data)
{
	int	cnt;

	cnt = 0;
	if (data->texture.no_path)
		cnt++;
	if (data->texture.so_path)
		cnt++;
	if (data->texture.we_path)
		cnt++;
	if (data->texture.ea_path)
		cnt++;
	if (data->texture.floor_color)
		cnt++;
	if (data->texture.ceiling_color)
		cnt++;
	return (cnt);
}

int	is_empty_line(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!is_space(line[i]) && line[i] != '\n' && line[i] != '\r')
			return (0);
		i++;
	}
	return (1);
}

void	get_config_info(t_data *data)
{
	data->map_info.parse_result = parse_config_line(data->map_info.line, data);
	if (data->map_info.parse_result == PARSE_SUCCESS)
		data->map_info.in_map = 1;
	else if (data->map_info.parse_result == PARSE_FAILED)
		free_exit(data, PARSE_FAILED, data->map_info.line);
}

void	get_map_info(t_data *data)
{
	if (data->map_info.is_empty)
	{
		data->map_info.map_finished = FINISHED;
		return ;
	}
	if (data->map_info.map_finished == FINISHED)
		free_exit(data, EMPTY_LINE, "empty line found in the map!\n");
	data->map_info.tmp_width = ft_strlen(data->map_info.line);
	if (data->map_info.tmp_width > 0
		&& data->map_info.line[data->map_info.tmp_width - 1] == '\n')
		data->map_info.tmp_width--;
	if (data->map_width < data->map_info.tmp_width)
		data->map_width = data->map_info.tmp_width;
	if (!data->map_info.map_finished)
		data->map_height++;
}

void	check_and_get_map_info(t_data *data)
{
	while ((data->map_info.line = get_next_line(data->fd)))
	{
		data->map_info.is_empty = is_empty_line(data->map_info.line);
		if (!data->map_info.in_map)
			get_config_info(data);
		if (data->map_info.in_map)
			get_map_info(data);
		free(data->map_info.line);
		data->file_rows++;
	}
}

int	map_init(t_data *data)
{
	data->fd = open(data->filename, O_RDONLY);
	if (data->fd < 0)
	{
		perror("Failed to open file");
		return (1);
	}
	check_and_get_map_info(data);
	if (all_dir_texture_is_valid(data) != 6)
		free_exit(data, LACK_OF_INFO, "lack of info\n");
	if (data->map_info.tmp == 2)
		free_exit(data, 1, "config error\n");
	close(data->fd);
	data->fd = open(data->filename, O_RDONLY);
	if (data->fd < 0)
		return (1);
	make_map(data->fd, data);
	check_map(data);
	close(data->fd);
	return (0);
}
