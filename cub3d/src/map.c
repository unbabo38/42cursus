/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   map.c                                             :+:      :+:    :+:    */
/*                                                   +:+ +:+         +:+      */
/*   By: username <username@student.42tokyo.jp>    #+#  +:+       +#+         */
/*                                               +#+#+#+#+#+   +#+            */
/*   Created: 2026/05/09 19:41:40 by username         #+#    #+#              */
/*   Updated: 2026/05/09 19:41:45 by username        ###   ########.fr        */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	free_tab(char **tab)
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
	char	**copy;
	int		i;

	copy = malloc(sizeof(char *) * (data->map_height + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < data->map_height)
	{
		// 既に幅が揃っているので、固定長(map_width)で確保しても良い
		copy[i] = ft_strdup(data->map[i]);
		if (!copy[i])
		{
			free_tab(copy);
			return (NULL);
		}
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

int	check_wall(t_data *data, char **map, int h, int w)
{
	if (h < 0 || h >= data->map_height || w < 0 || w >= data->map_width)
		return (1);
	char	position = map[h][w];

	if (position == '1' || position == 'v')
		return (0);
	else if (position == '0')
		map[h][w] = 'v';
	else if (is_space(position))
		return (1);
	if (check_wall(data, map, h + 1, w) ||
			check_wall(data, map, h - 1, w) ||
	check_wall(data, map, h, w + 1) ||
	check_wall(data, map, h, w - 1))
	return (1);
	return (0);
}

int	check_characters(t_data *data)
{
	int	h = 0;
	int	w = 0;
	int	cnt = 0;

	while (h < data->map_height)
	{
		w = 0;
		while (w < data->map_width)
		{
			char	position = data->map[h][w];

			printf("%c", position);
			//printf("map_contents:%c\n", data->map[h][w]);
			if (position == 'N' || position == 'S' || position == 'W' || position == 'E')
			{
				data->posX = w + 0.5;
				data->posY = h + 0.5;
				default_dir(position, data);
				data->map[h][w] = '0';
				cnt++;
			}
			if (position == 'K')
			{
				data->sprite.x = w + 0.5;
				data->sprite.y = h + 0.5;
				data->map[h][w] = '0';
				data->sprite.exist = 1;
			}
			// } else if (!(position == '0' || position == '1' || position != ' '))
			// {
			// 	printf("out_contents:%c\n", position);
			// 	return (1);
			// }
			w++;
		}
		printf("\n");
		h++;
	}
	printf("%d", cnt);
	if (cnt != 1)
		return (1);
	else
		return (0);
}

int	check_map(t_data *data)
{
	if (check_characters(data) != 0)
	{
		free_exit(data, 1, "Error\ncheck_characters failed\n");
		return (1);
	}
	char	**cmap = copy_map(data);

	if (check_wall(data, cmap, (int) data->posY, (int) data->posX) != 0)
	{
		free_tab(cmap);
		free_exit(data, 1, "Wall is not closed\n");
		return (1);
	}
	free_tab(cmap);
	printf("mapok\n");
	return (0);
}

void	copy_line(char *dst, char *src, t_data *data)
{
	int	i = 0;

	if (!src || !dst) return;
		while (i < data->map_width)
		{
			dst[i] = src[i];
			i++;
		}
	dst[i] = '\0';
}

void	make_map(int fd, t_data *data)
{
	int		i = 0;
	char	*line;
	int		in_map = 0;
	int		parse_result = 0;

	data->map = malloc(sizeof(char *) * (data->map_height + 1));
	if (!data->map)
	{
		return ;
	}
	while ((line = get_next_line(fd)))
	{
		if (!in_map)
		{
			parse_result = parse_config_line(line, data);
			if (parse_result == 0)
				in_map = 1;
		}
		// if (!in_map) else にせず、in_mapになった瞬間この下の処理を通す
		if (in_map && i < data->map_height)
		{
			data->map[i] = malloc(sizeof(char) * (data->map_width + 1));
			if (data->map[i])
				// 【重要】trimmedではなく、get_next_lineで取ったままの line を渡す
			copy_line(data->map[i], line, data);
			i++;
		}
		free(line);
	}
	data->map[i] = NULL;
}

int	all_dir_texture_is_valid(t_data *data)
{
	int	cnt = 0;

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
	int	i = 0;

	while (line[i])
	{
		if (is_space(line[i]))
			return (IS_SPACE);
		i++;
	}
	return (IS_NOT_SPACE);
}

void	get_config_info(t_data *data)
{
	data->map_info.parse_result = parse_config_line(data->map_info.line, data);
	if (data->map_info.parse_result == PARSE_SUCCESS)
		data->map_info.in_map = 1;
	if (data->map_info.parse_result == PARSE_FAILED)
		free_exit(data, PARSE_FAILED, data->map_info.line);
}

void	get_map_info(t_data *data)
{
	if (data->map_info.is_empty)
		data->map_info.map_finished = FINISHED;
	if (data->map_info.map_finished == FINISHED)
		free_exit(data, EMPTY_LINE, "empty line found in the map!\n");
	else
	{
		data->map_info.tmp_width = ft_strlen(data->map_info.line);
		if (data->map_info.tmp_width > 0 && data->map_info.line[data->map_info.tmp_width - 1] == '\n')
			data->map_info.tmp_width--;
		if (data->map_width < data->map_info.tmp_width)
			data->map_width = data->map_info.tmp_width;
		data->map_height++;
	}
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
	}
}

int	map_init(t_data *data)
{
	const char	*filename = "maps/validmap.cub";

	data->fd = open(filename, O_RDONLY);
	if (data->fd < 0)
	{
		perror("Failed to open file");
		return (1);
	}
	check_and_get_map_info(data);
	if (all_dir_texture_is_valid(data) != 6)
		free_exit(data, 1, "lack of info\n");
	if (data->map_info.tmp == 2)
		free_exit(data, 1, "config error\n");
	close(data->fd);
	data->fd = open(filename, O_RDONLY);
	if (data->fd < 0)
		return (1);
	make_map(data->fd, data);
	check_map(data);
	close(data->fd);
	return (0);
}
